/*
GetLMD Main Program
http://leidich-message-digest.blogspot.com
Copyright 2012-2014 Russell Leidich
May 22, 2014

This file is part of the GetLMD Library.

The GetLMD Library is free software: you can redistribute it
and/or modify it under the terms of the GNU Limited General Public License as
published by the Free Software Foundation, version 3.

The GetLMD Library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Limited
General Public License version 3 for more details.

You should have received a copy of the GNU Limited General Public License
version 3 along with the GetLMD Library (filename "COPYING").
If not, see http://www.gnu.org/licenses/ .
*/

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "build.h"
#include "file_sys.h"
#include "constant.h"
#include "lmd7.h"
#include "lmd8.h"
#include "lmd7.c"
#include "lmd8.c"

#define ALIGN_32_PAD 3
#define BLOCK_SIZE_LOG2 21
#define BLOCK_SIZE (1<<BLOCK_SIZE_LOG2)
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  #define PATH_SEPARATOR 0x5C
#else
  #define PATH_SEPARATOR 0x2F
#endif

enum error_code_enum{
  no_error,
  bad_syntax,
  out_of_memory,
  target_name_not_found,
  file_read_failed
};

char path_separator[1]={PATH_SEPARATOR};
/*
LMD constants, for historical purposes.
*/
#define LMD_A 0x7FFFFDCD
#define LMD_C0 0x7B98D2B0
#define LMD_X0 0x26711AAF
/*
These are the seeds of LMD2 -- a, x0, and c0, which were carefully selected for extra protection against 2-bit errors (1-bit errors being effectively
100% detectable).

They define a psuedorandom number generator with period 9,151,323,238,909,870,079 (just under 2^63).

The number of nonzero DWORDs before (x == 0) appears is 11,460,787,448, excluding the seeds themselves. (The 0 must be skipped in order to ensure robust
error detection, but there is no need to check for 0 on every loop; it can be handled via the string-DWORD-count-remaining counter.)

A 2-bit error in the first 263,837 DWORDs (1,055,348 bytes) cannot possibly result in the same partial digest as if either or both errors were not present,
although there is a tiny chance of at most 1 in (the pseudorandom number period given above) that they could result in the same final digest.
*/
#define LMD2_A 0xFE001000
#define LMD2_C0 0xC97A34B3
#define LMD2_X0 0x129E5CFA
/*
LMD3 constants.
*/
#define LMD3_A LMD2_A
#define LMD3_C0 0xDA6D32BA
#define LMD3_X0 0
/*
The digest always begins as a 64-bit 0. DIGEST_0 is just a shorthand way of saying "new digest".
*/
#define DIGEST_0 0ULL
/*
Here's the iterator, as a macro.

But first... if (x == 0) after iteration, then you must iterate again (which the macro handles).

PLEEEEEEEASE don't actually implement the "while (x == 0)" in your code. It's a waste of time and makes parallelizing difficult.  Instead, you can merge
this test with the loop counter for loading DWORDs from the string. Just be careful to handle the next zero, and the next, etc., appropriately. When 
processing very long strings, you might opt to switch to this slow macro after processing the first 3,132,319,170 DWORDs. See comments in the header
above. By the way, x and c will never both be 0 so long as they don't start out that way, so there's no danger of the "while (x == 0)" hanging the system, in
the event that you need to use it.

Frankly, check out ITERATE_NO_ZERO_CHECK below.

To parallelize this macro, just have a lookup table giving P values of (x, C) for each of P SIMD pipes -- each K iterations apart, starting from the seeds
above. That way, each pipe can work on K DWORDs of the string. If the string length is variable, then multiple sets of such values, in steps of K
iterations, may be needed. There are too many optimizations to discuss here. Anyway, when all pipes are done, add all the partial digests together, and
digest_finalize((the sum), (the last state of the iterand)) in a single thread to finish. Most importantly, validate your algorithm against this
inefficient C code.
*/
#define ITERATE1 \
  do { \
    product = LMD_A * (u64)x + (u64)c; \
    x = product & 0xFFFFFFFFULL; \
    c = product >> 32; \
  } while (x == 0);

#define ITERATE_2_OR_3 \
  do { \
    product = LMD2_A * (u64)x + (u64)c; \
    x = product & 0xFFFFFFFFULL; \
    c = product >> 32; \
  } while (x == 0);

#define ITERATE1_NO_ZERO_CHECK \
  product = LMD_A * (u64)x + (u64)c; \
  x = product & 0xFFFFFFFFULL; \
  c = product >> 32;

#define ITERATE_2_OR_3_NO_ZERO_CHECK \
  product = LMD2_A * (u64)x + (u64)c; \
  x = product & 0xFFFFFFFFULL; \
  c = product >> 32;

typedef struct {
/*
The iterand of the random number generator. Only a moron would manipulate these values using pointer indirection, but it works for demo purposes.
*/
  u32 x;
  u32 c;
} iterand_t;

void
iterator_init(u8 level,iterand_t *iterand_base) {
/*
Initialize the pseudorandom number generator.
*/
  if(level==1){
    iterand_base->x = LMD2_X0;
    iterand_base->c = LMD2_C0;
  }else if(level==2){
    iterand_base->x = LMD3_X0;
    iterand_base->c = LMD3_C0;
  }else{
    iterand_base->x = LMD_X0;
    iterand_base->c = LMD_C0;
  }
  return;
}

u64
string_digest_partial(u8 level,u32 *string_base,u32 string_size_in_dwords_minus_1,iterand_t *iterand_base,u64 digest_partial_previous) {
/*
Digest a chunk of a string, and add the result to the previous digest. This allows the digest to be parallelized, to some extent, with the arrival of chunks
of the string. With the help of some data tables containing the state of the iterand after various iteration counts, the chunks can even be digested as
they arrive out-of-order; there is no need to wait for all chunks, or even the first one, to arrive, before work can begin.

Simply set digest_partial_previous to the previous call's output value, or DIGEST_0 on the first call.

string_size_in_dwords_minus_1 < U32_MAX.
*/
  u32 c;
  u32 i;
  u64 product;
  u32 x;
  u64 digest_partial;

  digest_partial = digest_partial_previous;
/*
digest_partial is just the dot product of successive values of iterate() with the DWORDs of the string, modulo 2^64.
*/
  if(level){
    for(i = 0; i <= string_size_in_dwords_minus_1; i++) {
      x = iterand_base->x;
      c = iterand_base->c;
      ITERATE_2_OR_3
      iterand_base->x = x;
      iterand_base->c = c;
      digest_partial += (u64)x * (u64)string_base[i];
    }
  }else{
    for(i = 0; i <= string_size_in_dwords_minus_1; i++) {
      x = iterand_base->x;
      c = iterand_base->c;
      ITERATE1
      iterand_base->x = x;
      iterand_base->c = c;
      digest_partial += (u64)x * (u64)string_base[i];
    }
  }
  return digest_partial;
}

u64
digest_finalize (u8 level,u64 digest_partial,iterand_t *iterand_base) {
/*
Do the minimum amount of entropy amplification necessary to ensures that the digest is radically different for strings which differ by only one or a few
bits, or are all 0s or 1s of various lengths.
*/
  u64 product;
  u32 x;
  u32 c;

  x = iterand_base->x;
  c = iterand_base->c;
/*
Add c:x to digest_partial, as though the former were a 64-bit value, including the propagation of the carry from the low to high 32-bit sum. Then treat the
result as a new (x, c), and iterate thrice. Finally, add c:x back to digest_partial.

The point is to increase the number of digest bits which flip when a single bit flips in the message, which is the hardest case for a random string. We
iterate thrice because statistical analysis shows that, when we have a single-bit error in a random message, only about 18 bits change in digest_partial, as
originally input to this function; however, after feeding digest_partial into iterand, and iterating thrice, the bit difference count jumps to about 32,
making it maximally improbable that the digest could be accidentally compensated by its own bit errors so as to work out correctly despite an erroneous
string. The probability of such an event then drops down to about 5.4E(-20) on random input data, even in the presence of optimally bad noise (a 50% bit
inversion rate, in this case), which is _at_ the theoretical limit of 1 in 2^64. In the presence of even remotely reasonable normal noise levels (like 1%
inversion), the probability drops exponentially lower still, even though the digest is only 64 bits wide, because bits are highly unlikely to invert in
sufficient numbers in order to accomplish proper compensation.

The same analysis applied to single-bit errors in strings of 0s of various lengths -- an even harder case for robust error detection -- reveals a slightly
higher susceptibility, again in the presence of optimally bad noise (here, an 18% inversion rate), of 1.1E(-19).

Realistically, both probabilities are well below the level at which other nonrecoverable machine failures would dominate over data error detection failures,
so there is no need to waste time computing a 128-bit LMD.
*/
  digest_partial += ((u64)c << 32) | x;
  x = (u32)digest_partial;
  c = (u32)(digest_partial >> 32);
/*
Unlike when we were digesting the message, we won't be multiplying x by any information that we care to protect; we'll just be _adding_ the result of these
iterations to digest_partial. So don't waste time and space checking for (x == 0), hence ITERATE_NO_ZERO_CHECK instead of ITERATE. (Critically, the add from
digest_partial above could have made ((x, c) == (0, 0)), in which case we would never escape next ITERATE.)
*/
  if(level){
    ITERATE_2_OR_3_NO_ZERO_CHECK
    ITERATE_2_OR_3_NO_ZERO_CHECK
    ITERATE_2_OR_3_NO_ZERO_CHECK
  }else{
    ITERATE1_NO_ZERO_CHECK
    ITERATE1_NO_ZERO_CHECK
    ITERATE1_NO_ZERO_CHECK
  }
/*
We could theoretically stop here, and just say that the digest_partial _is_ product. But we use another add instruction because, while there are only about
2^62 unique values of product, there are 2^64 unique values of digest_partial. Because we want to avoid digest collisions to the maximum possible extent,
we add here, rather than copy.
*/
  digest_partial += product;
/*
Return digest_partial as the (final) digest. Don't waste time updating *iterand_base because we're done digesting a whole string (and x and c might both be
0, which would halt the iterator later and hang the system).
*/
  return digest_partial;
}

u64
string_checksum_partial(u8 mode,u32 *string_base,u32 string_size_in_dwords_minus_1,u64 checksum_partial_previous) {
/*
Analogous to string_digest_partial(), but for checksums.

string_size_in_dwords_minus_1 < U32_MAX.
*/
  u32 dword;
  u8 dword_count_even_status;
  u32 string_idx;
  u64 sum;

  sum=checksum_partial_previous;
  switch(mode){
  case 5:
    for(string_idx=0;string_idx<=string_size_in_dwords_minus_1;string_idx++){
      dword=string_base[string_idx];
      sum+=(u8)(dword);
      dword>>=8;
      sum+=(u8)(dword);
      dword>>=8;
      sum+=(u8)(dword);
      dword>>=8;
      sum+=dword;
    }
    break;
  case 6:
    for(string_idx=0;string_idx<=string_size_in_dwords_minus_1;string_idx++){
      dword=string_base[string_idx];
      sum+=(u16)(dword);
      dword>>=16;
      sum+=dword;
    }
    break;
  case 7:
    for(string_idx=0;string_idx<=string_size_in_dwords_minus_1;string_idx++){
      dword=string_base[string_idx];
      sum+=dword;
    }
    break;
  default:
    dword_count_even_status=string_size_in_dwords_minus_1&1;
    for(string_idx=0;string_idx<string_size_in_dwords_minus_1;string_idx+=2){
      sum+=((u64)(string_base[string_idx+1])<<32)+string_base[string_idx];
    }
    if(dword_count_even_status==0){
      sum+=string_base[string_idx];
    }
    break;
  }
  return sum;
}

void *align32_round_up(void *base){
  union {
    i64 base_as_i64;
    void *base_as_void_star;
  } base_union;

  base_union.base_as_void_star=base;
  base_union.base_as_i64=(-((-base_union.base_as_i64)>>2))<<2;
  return base_union.base_as_void_star;
}

void
string_lmd7_partial(u64 *block_idx_base,u32 *string_base,u32 string_size_in_4k_minus_1,u64 digest_base[LMD7_U64_COUNT]){
  u64 block_idx;
  u32 block_idx_local;
  u32 carry;
  u32 digest_idx;
  u64 digest_u64;
  u64 digest_u64_prev;
  u32 p_base[LMD7_U32_COUNT<<1];
  u32 string_idx;
  u32 z_base[LMD7_U32_COUNT];
  u32 z_idx;

  block_idx=*block_idx_base;
  block_idx_local=0;
  string_idx=0;
  do{
    memset(p_base,0,(size_t)(LMD7_SIZE<<1));
    p_base[0]=(u32)(block_idx);
    p_base[1]=(u32)(block_idx>>32);
    p_base[2]=1;
    p_base[LMD7_U32_COUNT]=(u32)(block_idx);
    p_base[LMD7_U32_COUNT+1]=(u32)(block_idx>>32);
    lmd_block_lmd7_get(string_base,string_idx,p_base,z_base);
    digest_idx=0;
    z_idx=0;
    carry=0;
    do{
      digest_u64_prev=((u64)(z_base[z_idx+1])<<32)+z_base[z_idx];
      z_idx+=2;
      digest_u64=digest_u64_prev+carry;
      carry=(digest_u64<digest_u64_prev);
      digest_u64_prev=digest_u64;
      digest_u64+=digest_base[digest_idx];
      digest_base[digest_idx]=digest_u64;
      carry+=(digest_u64<digest_u64_prev);
      digest_idx++;
    }while(digest_idx<=(LMD7_U64_COUNT-1));
    block_idx_local++;
    block_idx++;
    string_idx+=LMD7_BLOCK_U32_COUNT;
  }while(block_idx_local<=string_size_in_4k_minus_1);
  *block_idx_base=block_idx;
  return;
}

void
string_lmd8_partial(u64 *block_idx_base,u32 *string_base,u32 string_size_in_4k_minus_1,u64 digest_base[LMD8_U64_COUNT]){
  u64 block_idx;
  u32 block_idx_local;
  u32 carry;
  u32 digest_idx;
  u64 digest_u64;
  u64 digest_u64_prev;
  u32 p_base[LMD8_U32_COUNT<<1];
  u32 string_idx;
  u32 z_base[LMD8_U32_COUNT];
  u32 z_idx;

  block_idx=*block_idx_base;
  block_idx_local=0;
  string_idx=0;
  do{
    memset(p_base,0,(size_t)(LMD8_SIZE<<1));
    p_base[0]=(u32)(block_idx);
    p_base[1]=(u32)(block_idx>>32);
    p_base[2]=1;
    p_base[LMD8_U32_COUNT]=(u32)(block_idx);
    p_base[LMD8_U32_COUNT+1]=(u32)(block_idx>>32);
    lmd_block_lmd8_get(string_base,string_idx,p_base,z_base);
    digest_idx=0;
    z_idx=0;
    carry=0;
    do{
      digest_u64_prev=((u64)(z_base[z_idx+1])<<32)+z_base[z_idx];
      z_idx+=2;
      digest_u64=digest_u64_prev+carry;
      carry=(digest_u64<digest_u64_prev);
      digest_u64_prev=digest_u64;
      digest_u64+=digest_base[digest_idx];
      digest_base[digest_idx]=digest_u64;
      carry+=(digest_u64<digest_u64_prev);
      digest_idx++;
    }while(digest_idx<=(LMD8_U64_COUNT-1));
    block_idx_local++;
    block_idx++;
    string_idx+=LMD8_BLOCK_U32_COUNT;
  }while(block_idx_local<=string_size_in_4k_minus_1);
  *block_idx_base=block_idx;
  return;
}

enum error_code_enum
file_digest(u8 mode,u64 *digest_base,char *input_filename_base,u32 *block_base){
  u32 block_idx_max;
  u32 block_last_u32_count;
  u32 block_last_u32_idx_max;
  u32 block_tail_size;
  u64 digest0;
  u64 digest1;
  enum error_code_enum error_code;
  u64 file_block_idx;
  FILE *input_handle;
  i64 input_size;
  iterand_t iterand0;
  iterand_t iterand1;
  u8 level;

  memset(digest_base,0,(size_t)(LMD7_SIZE));
  digest0=DIGEST_0;
  digest1=DIGEST_0;
  error_code=no_error;
  if(mode<=2){
    level=mode;
  }else{
    level=1;
  }
  iterator_init(level,&iterand0);  
  iterator_init(2,&iterand1);
  if ((input_handle=fopen(input_filename_base,"rb"))){
    FSEEKO(input_handle,0,SEEK_END);
    if ((input_size=(i64)(FTELLO(input_handle)))||(9<=mode)){
      file_block_idx=input_size;
      if(mode<=4){
        if(mode>2){
          digest0=input_size;
          digest1=input_size;
        }
      }
      FSEEKO(input_handle,0,SEEK_SET);
      while ((error_code==no_error)&&(BLOCK_SIZE<=input_size)){
        if (fread((void *)(block_base),sizeof(u8),(size_t)(BLOCK_SIZE),input_handle)==(size_t)(BLOCK_SIZE)){
          if(mode<=4){
            digest0=string_digest_partial(level,block_base,(BLOCK_SIZE>>2)-1,&iterand0,digest0);
            if(mode==4){
              digest1=string_digest_partial(2,block_base,(BLOCK_SIZE>>2)-1,&iterand1,digest1);
            }
          }else if(mode<=8){
            digest0=string_checksum_partial(mode,block_base,(BLOCK_SIZE>>2)-1,digest0);
          }else if(mode==9){
            string_lmd7_partial(&file_block_idx,block_base,(BLOCK_SIZE>>LMD7_BLOCK_SIZE_LOG2)-1,digest_base);
          }else{
            string_lmd8_partial(&file_block_idx,block_base,(BLOCK_SIZE>>LMD8_BLOCK_SIZE_LOG2)-1,digest_base);
          }
          input_size-=BLOCK_SIZE;
        }else{
          error_code=file_read_failed;
        }
      }
      if ((error_code==no_error)&&(input_size||(9<=mode))){
        block_last_u32_count=(u32)(input_size>>2);
        if (input_size&3){
          block_base[block_last_u32_count]&=(1<<((input_size&3)<<3))-1;
          block_last_u32_count++;
        }
        if(9<=mode){
          block_tail_size=LMD7_BLOCK_SIZE;
          if(block_last_u32_count){
            block_tail_size=((-block_last_u32_count)&(LMD7_BLOCK_U32_COUNT-1))<<2;
          }
          memset(&block_base[block_last_u32_count],0,(size_t)(block_tail_size));
          block_idx_max=0;
          if(input_size){
            block_idx_max=((u32)(input_size)-1)>>LMD7_BLOCK_SIZE_LOG2;
          }
        }
        if ((!input_size)||(fread((void *)(block_base),sizeof(u8),(size_t)(input_size),input_handle)==(size_t)(input_size))){
          block_last_u32_idx_max=block_last_u32_count-1;
          if(mode<=4){
            digest0=string_digest_partial(level,block_base,block_last_u32_idx_max,&iterand0,digest0);
            if(mode==4){
              digest1=string_digest_partial(level,block_base,block_last_u32_idx_max,&iterand1,digest1);
            }
          }else if(mode<=8){
            digest0=string_checksum_partial(mode,block_base,block_last_u32_idx_max,digest0);
          }else{
            if(mode==9){
              string_lmd7_partial(&file_block_idx,block_base,block_idx_max,digest_base);
            }else{
              string_lmd8_partial(&file_block_idx,block_base,block_idx_max,digest_base);
            }
          }
        }else{
          error_code=file_read_failed;
        }
      }
    }
    fclose(input_handle);
  }else{
    error_code=target_name_not_found;
  }
  if(mode<=4){
    *digest_base=digest_finalize(level,digest0,&iterand0);
    if(mode==4){
      digest_base[1]=digest_finalize(2,digest1,&iterand1);
    }
  }else if(mode<=8){
    *digest_base=digest0;
  }
  return error_code;
}

void
printf_ignored(char *ignored_string_base,char *target_name_base){
  printf("IGNORED_%s: %s\n",ignored_string_base,target_name_base);
}

void
printf_ignored_empty_folder(char *target_name_base){
  printf_ignored("EMPTY_FOLDER",target_name_base);
  return;
}

void
printf_ignored_inaccessible(char *target_name_base){
  printf_ignored("INACCESSIBLE",target_name_base);
  return;
}

void
printf_ignored_symlink(char *target_name_base){
  printf_ignored("SYMLINK",target_name_base);
  return;
}

enum error_code_enum
file_or_directory_digest(u8 mode,u64 sum_of_digests_base[LMD7_U64_COUNT],i64 *target_name_size_max_base,char **target_name_base_base,u32 *block_base,u8 *directory_status_base,u8 *ignored_status_base,u8 *file_processed_status_base){
  u32 carry;
  u64 digest_base[LMD7_U64_COUNT];
  u32 digest_idx;
  u64 digest_u64;
  u64 digest_u64_prev;
  struct dirent *directory_base;
  u8 directory_empty_status;
  DIR *directory_handle;
  char *directory_member_name_base;
  i64 directory_member_name_size;
  u8 directory_processed_status;
  enum error_code_enum error_code;
  u32 *input_buffer;
  char *target_name_base;
  i64 target_name_size;
  i64 target_name_size_previous;
  struct stat target_stat;

  target_name_base=*target_name_base_base;
  directory_processed_status=0;
  directory_empty_status=0;
  if ((directory_handle=opendir(target_name_base))){
    target_name_size_previous=(i64)(strlen(target_name_base))+1;
    *directory_status_base=1;
    error_code=no_error;
    directory_empty_status=1;
    while ((error_code==no_error)&&(directory_base=readdir(directory_handle))){
      directory_member_name_base=directory_base->d_name;
      if (strcmp(directory_member_name_base,".")&&strcmp(directory_member_name_base,"..")){
        directory_empty_status=0;
        directory_member_name_size=strlen(directory_member_name_base);
        target_name_size=target_name_size_previous+1+directory_member_name_size;
        if (target_name_size>*target_name_size_max_base){
          if ((target_name_base=(char *)(realloc((void *)(target_name_base),(size_t)(target_name_size))))){
            *target_name_size_max_base=target_name_size;
            *target_name_base_base=target_name_base;
          }else{
            error_code=out_of_memory;
            break;
          }
        }
        strcat(target_name_base,path_separator);
        strcat(target_name_base,(const char *)(directory_member_name_base));
/*
We have to use lstat() instead of just inspecting directory_base->d_type because DT_LNK is not consistently reported for directories, even within the same Ubuntu installation. But lstat() doesn't work in Windows, where links (shortcuts) appear to operate like files rather than directories, and therefore carry no risk of self-referential loops and thus do not need to be ignored.
*/
        #ifndef WINDOWS
          if(!lstat(target_name_base,&target_stat)){
            if((target_stat.st_mode&S_IFLNK)!=S_IFLNK){
        #endif
              error_code=file_or_directory_digest(mode,sum_of_digests_base,target_name_size_max_base,target_name_base_base,block_base,directory_status_base,ignored_status_base,file_processed_status_base);
              target_name_base=*target_name_base_base;
        #ifndef WINDOWS
            }else{
              printf_ignored_symlink(target_name_base);
              *ignored_status_base=1;
            }
          }else{
            printf_ignored_inaccessible(target_name_base);
            *ignored_status_base=1;
          }
        #endif
        target_name_base[target_name_size_previous-1]=0;
      }
    }
    closedir(directory_handle);
    directory_processed_status=1;
  }else{
    error_code=file_digest(mode,digest_base,target_name_base,block_base);
    if(error_code==no_error){
      *file_processed_status_base=1;
      if(mode<=8){
        *sum_of_digests_base+=*digest_base;
        sum_of_digests_base[1]+=digest_base[1];
      }else{
        digest_idx=0;
        carry=0;
        do{
          digest_u64_prev=digest_base[digest_idx];
          digest_u64=digest_u64_prev+carry;
          carry=(digest_u64<digest_u64_prev);
          digest_u64_prev=digest_u64;
          digest_u64=sum_of_digests_base[digest_idx];
          digest_u64+=digest_u64_prev;
          sum_of_digests_base[digest_idx]=digest_u64;
          carry+=(digest_u64<digest_u64_prev);
          digest_idx++;
        }while(digest_idx<=(LMD7_U64_COUNT-1));
      }
      if(mode<=4){
        if(mode!=4){
          printf("%08X%08X",(u32)(*digest_base>>32),(u32)(*digest_base));
        }else{
          printf("%08X%08X%08X%08X",(u32)(*digest_base>>32),(u32)(*digest_base),(u32)(digest_base[1]>>32),(u32)(digest_base[1]));
        }
      }else{
        switch(mode){
        case 5:
          *digest_base=(u8)(*digest_base);
          printf("%02X",(u32)(*digest_base));
          break;
        case 6:
          *digest_base=(u16)(*digest_base);
          printf("%04X",(u32)(*digest_base));
          break;
        case 7:
          *digest_base=(u32)(*digest_base);
          printf("%08X",(u32)(*digest_base));
          break;
        case 8:
          printf("%08X%08X",(u32)(*digest_base>>32),(u32)(*digest_base));
          break;
        case 9:
          digest_idx=LMD7_U64_COUNT-1;
          do{
            printf("%08X%08X",(u32)(digest_base[digest_idx]>>32),(u32)(digest_base[digest_idx]));
            digest_idx--;
          }while(digest_idx<=(LMD7_U64_COUNT-1));
          break;
        default:
          digest_idx=LMD8_U64_COUNT-1;
          do{
            printf("%08X%08X",(u32)(digest_base[digest_idx]>>32),(u32)(digest_base[digest_idx]));
            digest_idx--;
          }while(digest_idx<=(LMD8_U64_COUNT-1));
          break;
        }
      }
    }
  }
  if (directory_empty_status||((!directory_processed_status)&&((error_code==target_name_not_found)||(error_code==file_read_failed)))){
    if (directory_empty_status){
      printf_ignored_empty_folder(target_name_base);
    }else{
      printf_ignored_inaccessible(target_name_base);
    }
    *ignored_status_base=1;
    error_code=no_error;
  }else if (directory_empty_status||((!directory_processed_status)&&(error_code!=out_of_memory))){
    printf(": %s\n",target_name_base);
  }
  return error_code;
}

int main(int argc, char *argv[]){
  u32 *block_base;
  u8 directory_status;
  enum error_code_enum error_code;
  u8 file_processed_status;
  u8 ignored_status;
  u8 mode;
  u64 sum_of_digests_base[LMD7_U64_COUNT];
  u32 sum_of_digests_idx;
  char *target_name_base;
  i64 target_name_size_max;

  memset(sum_of_digests_base,0,(size_t)(LMD7_SIZE));
  directory_status=0;
  error_code=no_error;
  file_processed_status=0;
  ignored_status=0;
  mode=3;
  if(argc==3){
    error_code=bad_syntax;
    if(strlen(argv[2])==1){
      mode=(*(char *)(argv[2])|0x20)-'1';
      if(mode<=8){
        argc--;
      }else if(mode==('a'-'1')){
        mode=9;
        argc--;
      }else if(mode==('b'-'1')){ 
        mode=10;
        argc--;
      }
    }
  }
  if(argc==2){
    target_name_size_max=(i64)(strlen((const char *)(argv[1])))+1;
    target_name_base=(char *)(malloc((size_t)(target_name_size_max)));
    block_base=(u32 *)(align32_round_up(malloc((size_t)(BLOCK_SIZE+ALIGN_32_PAD))));
    if (target_name_base&&block_base){
      memcpy(target_name_base,argv[1],(size_t)(target_name_size_max));
      if ((error_code=file_or_directory_digest(mode,sum_of_digests_base,&target_name_size_max,&target_name_base,block_base,&directory_status,&ignored_status,&file_processed_status))==no_error){
        if (directory_status){
          if (file_processed_status){
            if(mode<=4){
              if(mode<=2){
                printf("%08X%08X: SUM_OF_RAW_LMD%d\n",(u32)(*sum_of_digests_base>>32),(u32)(*sum_of_digests_base),mode+1);
              }else{
                printf("%08X%08X",(u32)(*sum_of_digests_base>>32),(u32)(*sum_of_digests_base));
                if(mode==4){
                  printf("%08X%08X",(u32)(sum_of_digests_base[1]>>32),(u32)(sum_of_digests_base[1]));
                }
                printf(": SUM_OF_FILE_SIZE_HASHED_LMD2");
                if(mode==4){
                  printf(":SUM_OF_FILE_SIZE_HASHED_LMD3");
                }
                printf("\n");
              }
            }else if(mode<=8){
              switch(mode){
              case 5:
                *sum_of_digests_base=(u8)(*sum_of_digests_base);
                printf("%02X: 8",(u32)(*sum_of_digests_base));
                break;
              case 6:
                *sum_of_digests_base=(u16)(*sum_of_digests_base);
                printf("%04X: 16",(u32)(*sum_of_digests_base));
                break;
              case 7:
                *sum_of_digests_base=(u32)(*sum_of_digests_base);
                printf("%08X: 32",(u32)(*sum_of_digests_base));
                break;
              default:
                printf("%08X%08X: 64",(u32)(*sum_of_digests_base>>32),(u32)(*sum_of_digests_base));
                break;
              }
              printf("_BIT_CHECKSUM\n");
            }else{
              if(mode==9){
                sum_of_digests_idx=LMD7_U64_COUNT-1;
                do{
                  printf("%08X%08X",(u32)(sum_of_digests_base[sum_of_digests_idx]>>32),(u32)(sum_of_digests_base[sum_of_digests_idx]));
                  sum_of_digests_idx--;
                }while(sum_of_digests_idx<=(LMD7_U64_COUNT-1));
              }else{
                sum_of_digests_idx=LMD8_U64_COUNT-1;
                do{
                  printf("%08X%08X",(u32)(sum_of_digests_base[sum_of_digests_idx]>>32),(u32)(sum_of_digests_base[sum_of_digests_idx]));
                  sum_of_digests_idx--;
                }while(sum_of_digests_idx<=(LMD8_U64_COUNT-1));
              }
              mode-=2;
              printf(": SUM_OF_FILE_SIZE_HASHED_LMD%d\n",mode);
            }
            if (ignored_status){
              printf("\nERROR_IGNORED_SOME_TARGETS: Search \"IGNORED\" above to see errors.\n");
            }
          }
        }else if (file_processed_status==0){
          printf("\nERROR_BAD_TARGET: Target is inaccessible or does not exist.\n");
        }
      }
    }else{
      error_code=out_of_memory;
    }
    free(block_base);
    free(target_name_base);
  }else{
    printf("\nGetLMD build %d\nComputes the LMD/LMD2/LMD3/LMD7/LMD8/Checksum of a file or directory.\nCopyright (c) 2012-2014 Russell Leidich, all rights reserved.\nhttp://leidich-message-digest.blogspot.com\n\n",BUILD_NUMBER);
    printf("Syntax: getlmd [file or folder, no wildcards, symlinks ignored] [mode]\n\n");
    printf("where [mode] is one of the following:\n\n");
    printf("  1: (insecure) raw LMD\n");
    printf("  2: (insecure) raw LMD2\n");
    printf("  3: (insecure) raw LMD3\n");
    printf("  4: (insecure) LMD2 hashed with file size (default)\n");
    printf("  5: (insecure) LMD2:LMD3 hashed with file size\n");
    printf("  6: (insecure) raw 8-bit checksum\n");
    printf("  7: (insecure) raw 16-bit checksum\n");
    printf("  8: (insecure) raw 32-bit checksum\n");
    printf("  9: (insecure) raw 64-bit checksum\n");
    printf("  A: (secure) LMD7 hashed with file size\n");
    printf("  B: (secure) LMD8 hashed with file size\n\n");
    printf("Without file size hashing, padding with 0s can sometimes result in the same\n");
    printf("hash. \"Secure\" means that producing 2 different files with the same hash is\n");
    printf("physically impossible as far as I can tell (but it can be slow). Metadata such\n");
    printf("as the file name, permissions, and timestamp are NOT hashed.\n");
    error_code=bad_syntax;
  }
  if (error_code==out_of_memory){
    printf("\nERROR_OUT_OF_MEMORY.\n");
  }
  return (int)(error_code);
}
