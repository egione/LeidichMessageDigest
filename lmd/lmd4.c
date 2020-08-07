/*
Leidich Message Digest
http://leidich-message-digest.blogspot.com
Copyright 2013-2014 Russell Leidich
March 2, 2014

This file is part of the Leidich Message Digest Library.

The Leidich Message Digest Library is free software: you can redistribute it
and/or modify it under the terms of the GNU Limited General Public License as
published by the Free Software Foundation, version 3.

The Leidich Message Digest Library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Limited
General Public License version 3 for more details.

You should have received a copy of the GNU Limited General Public License
version 3 along with the Leidich Message Digest Library (filename "COPYING").
If not, see http://www.gnu.org/licenses/ .
*/
void
lmd_block_lmd4_get(u32 block_base[LMD4_BLOCK_U32_COUNT],ULONG block_idx_min,u32 p_base[LMD4_U32_COUNT<<1],u32 z_base[LMD4_U32_COUNT]){
/*
Compute the LMD4 of a block of size LMD4_BLOCK_SIZE. The algo is described at:

  http://leidich-message-digest.blogspot.com/2012/04/the-lmd4-lmd5-and-lmd6-secure-hashes.html

In:

  *block_base is self-explanatory.

  block_idx_min offsets block_base.

  *p_base is as follows, with each of the following structures containing (LMD4_U32_COUNT>>1) (u32)s:

    x0 for LMD4_A0
    c0 for LMD4_A0
    x1 for LMD4_A1
    c1 for LMD4_A1

  *z_base is undefined.

Out:

  *p_base is undefined.

  *z_base contains z, which is the LMD4 of the block.
*/
  ULONG block_idx;
  u32 i;
  i64 p;
  u32 *q_base;
  u32 *r_base;
  u32 s_base[LMD4_U32_COUNT];
  u32 *temp_base;
  u32 x0;
  u32 x1;
  u32 x2;

  q_base=&p_base[LMD4_U32_COUNT];
  r_base=s_base;
  for(i=0;i<=(LMD4_BLOCK_U32_COUNT-(LMD4_U32_COUNT>>1));i+=LMD4_U32_COUNT>>1){
    memcpy((void *)(z_base),(const void *)(&p_base[LMD4_U32_COUNT>>1]),(size_t)(LMD4_SIZE>>1));
    block_idx=i+block_idx_min;
    z_base[LMD4_U32_COUNT>>1]=p_base[0]^block_base[block_idx]^q_base[0];
    z_base[(LMD4_U32_COUNT>>1)+1]=p_base[1]^block_base[block_idx+1]^q_base[1];
    z_base[(LMD4_U32_COUNT>>1)+2]=p_base[2]^block_base[block_idx+2]^q_base[2];
    z_base[(LMD4_U32_COUNT>>1)+3]=p_base[3]^block_base[block_idx+3]^q_base[3];
    x0=z_base[4];
    p=z_base[3];
    p-=x0<<(LMD4_A0_SHIFT_2&U32_BIT_MAX);
    p-=x0<<(LMD4_A0_SHIFT_1&U32_BIT_MAX);
    p-=x0<<(LMD4_A0_SHIFT_0&U32_BIT_MAX);
    z_base[3]=(u32)(p);
    x1=x0;
    x0=z_base[5];
    p=(p>>U32_BITS)+z_base[4];
    p-=(x0<<(LMD4_A0_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_2&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_1&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_1&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_0&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_0&U32_BIT_MAX)));
    z_base[4]=(u32)(p);
    x1=x0;
    x0=z_base[6];
    p=(p>>U32_BITS)+z_base[5];
    p-=(x0<<(LMD4_A0_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_2&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_1&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_1&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_0&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_0&U32_BIT_MAX)));
    z_base[5]=(u32)(p);
    x1=x0;
    x0=z_base[7];
    p=(p>>U32_BITS)+z_base[6];
    p-=(x0<<(LMD4_A0_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_2&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_1&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_1&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A0_SHIFT_0&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A0_SHIFT_0&U32_BIT_MAX)));
    z_base[6]=(u32)(p);
    p=(p>>U32_BITS)+z_base[7];
    p-=x0>>(U32_BITS-(LMD4_A0_SHIFT_2&U32_BIT_MAX));
    p-=x0>>(U32_BITS-(LMD4_A0_SHIFT_1&U32_BIT_MAX));
    p-=x0>>(U32_BITS-(LMD4_A0_SHIFT_0&U32_BIT_MAX));
    z_base[7]=(u32)(p);
    temp_base=p_base;
    p_base=z_base;
    z_base=temp_base;
    memcpy((void *)(r_base),(const void *)(&q_base[LMD4_U32_COUNT>>1]),(size_t)(LMD4_SIZE>>1));
    block_idx=(i^(LMD4_BLOCK_U32_COUNT>>1))+block_idx_min;
    r_base[LMD4_U32_COUNT>>1]=q_base[0]^block_base[block_idx];
    r_base[(LMD4_U32_COUNT>>1)+1]=q_base[1]^block_base[block_idx+1];
    r_base[(LMD4_U32_COUNT>>1)+2]=q_base[2]^block_base[block_idx+2];
    r_base[(LMD4_U32_COUNT>>1)+3]=q_base[3]^block_base[block_idx+3];
    x0=r_base[4];
    p=r_base[2];
    p-=x0<<(LMD4_A1_SHIFT_2&U32_BIT_MAX);
    r_base[2]=(u32)(p);
    x1=x0;
    x0=r_base[5];
    p=(p>>U32_BITS)+r_base[3];
    p-=(x0<<(LMD4_A1_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A1_SHIFT_2&U32_BIT_MAX)));
    p-=x1<<(LMD4_A1_SHIFT_1&U32_BIT_MAX);    
    p-=x1<<(LMD4_A1_SHIFT_0&U32_BIT_MAX);
    r_base[3]=(u32)(p);
    x2=x1;
    x1=x0;
    x0=r_base[6];
    p=(p>>U32_BITS)+r_base[4];
    p-=(x0<<(LMD4_A1_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A1_SHIFT_2&U32_BIT_MAX)));
    p-=(x1<<(LMD4_A1_SHIFT_1&U32_BIT_MAX))|(x2>>(U32_BITS-(LMD4_A1_SHIFT_1&U32_BIT_MAX)));
    p-=(x1<<(LMD4_A1_SHIFT_0&U32_BIT_MAX))|(x2>>(U32_BITS-(LMD4_A1_SHIFT_0&U32_BIT_MAX)));
    r_base[4]=(u32)(p);
    x2=x1;
    x1=x0;
    x0=r_base[7];
    p=(p>>U32_BITS)+r_base[5];
    p-=(x0<<(LMD4_A1_SHIFT_2&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A1_SHIFT_2&U32_BIT_MAX)));
    p-=(x1<<(LMD4_A1_SHIFT_1&U32_BIT_MAX))|(x2>>(U32_BITS-(LMD4_A1_SHIFT_1&U32_BIT_MAX)));
    p-=(x1<<(LMD4_A1_SHIFT_0&U32_BIT_MAX))|(x2>>(U32_BITS-(LMD4_A1_SHIFT_0&U32_BIT_MAX)));
    r_base[5]=(u32)(p);
    p=(p>>U32_BITS)+r_base[6];
    p-=(x0>>(U32_BITS-(LMD4_A1_SHIFT_2&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A1_SHIFT_1&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A1_SHIFT_1&U32_BIT_MAX)));
    p-=(x0<<(LMD4_A1_SHIFT_0&U32_BIT_MAX))|(x1>>(U32_BITS-(LMD4_A1_SHIFT_0&U32_BIT_MAX)));
    r_base[6]=(u32)(p);
    p=(p>>U32_BITS)+r_base[7];
    p-=(x0>>(U32_BITS-(LMD4_A1_SHIFT_1&U32_BIT_MAX)));
    p-=(x0>>(U32_BITS-(LMD4_A1_SHIFT_0&U32_BIT_MAX)));
    r_base[7]=(u32)(p);
    temp_base=q_base;
    q_base=r_base;
    r_base=temp_base;
  }
  p=0;
  for(i=0;i<=(LMD4_U32_COUNT-1);i++){
    p>>=U32_BITS;
    p+=p_base[i];
    p+=p_base[(i+LMD4_U32_COUNT)^(LMD4_U32_COUNT>>1)];
    z_base[i]=(u32)(p);
  }
  return;
}
