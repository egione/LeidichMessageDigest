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
lmd_block_lmd6_get64(u64 block_base[LMD6_BLOCK_U64_COUNT],ULONG block_idx_min,u64 p_base[LMD6_U64_COUNT<<1],u64 z_base[LMD6_U64_COUNT]){
/*
Compute the LMD6 of a block of size LMD6_BLOCK_SIZE. The algo is described at:

  http://leidich-message-digest.blogspot.com/2012/04/the-lmd4-lmd5-and-lmd6-secure-hashes.html

In:

  *block_base is self-explanatory.

  block_idx_min offsets block_base.

  *p_base is as follows, with each of the following structures containing (LMD6_U64_COUNT>>1) (u64)s:

    x0 for LMD6_A0.
    c0 for LMD6_A0.
    x1 for LMD6_A1.
    c1 for LMD6_A1.

  *z_base is undefined.

Out:

  *p_base is undefined.

  *z_base contains z, which is the LMD6 of the block.
*/
  ULONG block_idx;
  u32 borrow;
  u32 carry;
  u32 i;
  u32 j;
  u64 p;
  u64 p_prev;
  u64 *q_base;
  u64 *r_base;
  u64 s_base[LMD6_U64_COUNT];
  u64 *temp_base;
  u64 x0;
  u64 x1;
  u64 x2;
  u64 x3;

  q_base=&p_base[LMD6_U64_COUNT];
  r_base=s_base;
  for(i=0;i<=(LMD6_BLOCK_U64_COUNT-(LMD6_U64_COUNT>>1));i+=LMD6_U64_COUNT>>1){
    memcpy((void *)(z_base),(const void *)(&p_base[LMD6_U64_COUNT>>1]),(size_t)(LMD6_SIZE>>1));
    block_idx=i+block_idx_min;
    z_base[LMD6_U64_COUNT>>1]=p_base[0]^block_base[block_idx]^q_base[0];
    z_base[(LMD6_U64_COUNT>>1)+1]=p_base[1]^block_base[block_idx+1]^q_base[1];
    z_base[(LMD6_U64_COUNT>>1)+2]=p_base[2]^block_base[block_idx+2]^q_base[2];
    z_base[(LMD6_U64_COUNT>>1)+3]=p_base[3]^block_base[block_idx+3]^q_base[3];
    z_base[(LMD6_U64_COUNT>>1)+4]=p_base[4]^block_base[block_idx+4]^q_base[4];
    z_base[(LMD6_U64_COUNT>>1)+5]=p_base[5]^block_base[block_idx+5]^q_base[5];
    z_base[(LMD6_U64_COUNT>>1)+6]=p_base[6]^block_base[block_idx+6]^q_base[6];
    z_base[(LMD6_U64_COUNT>>1)+7]=p_base[7]^block_base[block_idx+7]^q_base[7];
    x0=z_base[8];
    p_prev=z_base[6];
    p=p_prev-(x0<<(LMD6_A0_SHIFT_2&U64_BIT_MAX));
    borrow=(p_prev<p);
    z_base[6]=p;
    x1=x0;
    x0=z_base[9];
    p_prev=z_base[7];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    p_prev=p;   
    p-=(x0<<(LMD6_A0_SHIFT_2&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A0_SHIFT_2&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=x1<<(LMD6_A0_SHIFT_1&U64_BIT_MAX);
    borrow+=(p_prev<p);
    p_prev=p;
    p-=x1<<(LMD6_A0_SHIFT_0&U64_BIT_MAX);
    borrow+=(p_prev<p);
    z_base[7]=p;
    for(j=8;j<=13;j++){
      x2=x1;
      x1=x0;
      x0=z_base[j+2];
      p=x2-borrow;
      borrow=(x2<p);
      p_prev=p;
      p-=(x0<<(LMD6_A0_SHIFT_2&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A0_SHIFT_2&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      p_prev=p;
      p-=(x1<<(LMD6_A0_SHIFT_1&U64_BIT_MAX))|(x2>>(U64_BITS-(LMD6_A0_SHIFT_1&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      p_prev=p;
      p-=(x1<<(LMD6_A0_SHIFT_0&U64_BIT_MAX))|(x2>>(U64_BITS-(LMD6_A0_SHIFT_0&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      z_base[j]=p;
    }
    p_prev=z_base[14];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    p_prev=p;
    p-=x0>>(U64_BITS-(LMD6_A0_SHIFT_2&U64_BIT_MAX));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=(x0<<(LMD6_A0_SHIFT_1&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A0_SHIFT_1&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=(x0<<(LMD6_A0_SHIFT_0&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A0_SHIFT_0&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    z_base[14]=p;
    p=z_base[15]-borrow;
    p-=x0>>(U64_BITS-(LMD6_A0_SHIFT_1&U64_BIT_MAX));
    p-=x0>>(U64_BITS-(LMD6_A0_SHIFT_0&U64_BIT_MAX));
    z_base[15]=p;
    temp_base=p_base;
    p_base=z_base;
    z_base=temp_base;
    memcpy((void *)(r_base),(const void *)(&q_base[LMD6_U64_COUNT>>1]),(size_t)(LMD6_SIZE>>1));
    block_idx=(i^(LMD6_BLOCK_U64_COUNT>>1))+block_idx_min;
    r_base[LMD6_U64_COUNT>>1]=q_base[0]^block_base[block_idx];
    r_base[(LMD6_U64_COUNT>>1)+1]=q_base[1]^block_base[block_idx+1];
    r_base[(LMD6_U64_COUNT>>1)+2]=q_base[2]^block_base[block_idx+2];
    r_base[(LMD6_U64_COUNT>>1)+3]=q_base[3]^block_base[block_idx+3];
    r_base[(LMD6_U64_COUNT>>1)+4]=q_base[4]^block_base[block_idx+4];
    r_base[(LMD6_U64_COUNT>>1)+5]=q_base[5]^block_base[block_idx+5];
    r_base[(LMD6_U64_COUNT>>1)+6]=q_base[6]^block_base[block_idx+6];
    r_base[(LMD6_U64_COUNT>>1)+7]=q_base[7]^block_base[block_idx+7];
    x0=r_base[8];
    p_prev=r_base[5];
    p=p_prev-(x0<<(LMD6_A1_SHIFT_2&U64_BIT_MAX));
    borrow=(p_prev<p);
    r_base[5]=p;
    x1=x0;
    x0=r_base[9];
    p_prev=r_base[6];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    p_prev=p;
    p-=(x0<<(LMD6_A1_SHIFT_2&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A1_SHIFT_2&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=x1<<(LMD6_A1_SHIFT_1&U64_BIT_MAX);
    borrow+=(p_prev<p);
    r_base[6]=p;
    x2=x1;
    x1=x0;
    x0=r_base[10];
    p_prev=r_base[7];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    p_prev=p;
    p-=(x0<<(LMD6_A1_SHIFT_2&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A1_SHIFT_2&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=(x1<<(LMD6_A1_SHIFT_1&U64_BIT_MAX))|(x2>>(U64_BITS-(LMD6_A1_SHIFT_1&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=x2<<(LMD6_A1_SHIFT_0&U64_BIT_MAX);
    borrow+=(p_prev<p);
    r_base[7]=p;
    for(j=8;j<=12;j++){
      x3=x2;
      x2=x1;
      x1=x0;
      x0=r_base[j+3];
      p=x3-borrow;
      borrow=(x3<p);
      p_prev=p;
      p-=(x0<<(LMD6_A1_SHIFT_2&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A1_SHIFT_2&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      p_prev=p;
      p-=(x1<<(LMD6_A1_SHIFT_1&U64_BIT_MAX))|(x2>>(U64_BITS-(LMD6_A1_SHIFT_1&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      p_prev=p;
      p-=(x2<<(LMD6_A1_SHIFT_0&U64_BIT_MAX))|(x3>>(U64_BITS-(LMD6_A1_SHIFT_0&U64_BIT_MAX)));
      borrow+=(p_prev<p);
      r_base[j]=p;
    }
    p_prev=r_base[13];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    p_prev=p;
    p-=x0>>(U64_BITS-(LMD6_A1_SHIFT_2&U64_BIT_MAX));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=(x0<<(LMD6_A1_SHIFT_1&U64_BIT_MAX))|(x1>>(U64_BITS-(LMD6_A1_SHIFT_1&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    x3=x2;
    x2=r_base[14];
    p_prev=p;
    p-=(x2<<(LMD6_A1_SHIFT_0&U64_BIT_MAX))|(x3>>(U64_BITS-(LMD6_A1_SHIFT_0&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    r_base[13]=p;
    p_prev=r_base[14];
    p=p_prev-borrow;
    borrow=(p_prev<p);
    x3=x2;
    x2=r_base[15];
    p_prev=p;
    p-=x0>>(U64_BITS-(LMD6_A1_SHIFT_1&U64_BIT_MAX));
    borrow+=(p_prev<p);
    p_prev=p;
    p-=(x2<<(LMD6_A1_SHIFT_0&U64_BIT_MAX))|(x3>>(U64_BITS-(LMD6_A1_SHIFT_0&U64_BIT_MAX)));
    borrow+=(p_prev<p);
    r_base[14]=p;
    p=r_base[15]-borrow;
    p-=x2>>(U64_BITS-(LMD6_A1_SHIFT_0&U64_BIT_MAX));
    r_base[15]=p;
    temp_base=q_base;
    q_base=r_base;
    r_base=temp_base;
  }
  carry=0;
  for(i=0;i<=(LMD6_U64_COUNT-1);i++){
    p=p_base[i]+carry;
    carry=(p<carry);
    p_prev=p;
    p+=p_base[(i+LMD6_U64_COUNT)^(LMD6_U64_COUNT>>1)];
    carry+=(p<p_prev);
    z_base[i]=p;
  }
  return;
}
