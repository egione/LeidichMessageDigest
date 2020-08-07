/*
Leidich Message Digest
http://leidich-message-digest.blogspot.com
Copyright 2013 Russell Leidich
February 15, 2013

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
/*
Coefficient definitions for documentation purposes:

  LMD6_A0==(2^0x200-2^0x1F2-2^0x1F0-2^0x1AB)
  LMD6_A1==(2^0x200-2^0x1E1-2^0x194-2^0x16A)

These numbers have been selected because they are easy to multiply by, and:

  LMD6_A0*2^0x200-1 is prime,
  LMD6_A1*2^0x200-1 is prime,
  LMD6_A0*2^0x1FF-1 is prime,
  and LMD6_A1*2^0x1FF-1 is prime.

which implies that the cycle lengths of the iterations (x,c)-->((LMD6_A0*x+c) MOD 2^0x400) and (x,c)-->((LMD6_A1*x+c) MOD 2^0x400) are the last 2 numbers above, respectively.

The exponents above are defined as follows:
*/
#define LMD6_A0_SHIFT_0 0x1F2
#define LMD6_A0_SHIFT_1 0x1F0
#define LMD6_A0_SHIFT_2 0x1AB
#define LMD6_A1_SHIFT_0 0x1E1
#define LMD6_A1_SHIFT_1 0x194
#define LMD6_A1_SHIFT_2 0x16A
#define LMD6_BIT_COUNT (1<<(LMD6_SIZE_LOG2+U8_BITS_LOG2))
#define LMD6_BLOCK_SIZE 0x1000
#define LMD6_BLOCK_U32_COUNT (LMD6_BLOCK_SIZE>>U32_SIZE_LOG2)
#define LMD6_BLOCK_U64_COUNT (LMD6_BLOCK_SIZE>>U64_SIZE_LOG2)
#define LMD6_SIZE (1<<LMD6_SIZE_LOG2)
#define LMD6_SIZE_LOG2 7
#define LMD6_U32_COUNT (LMD6_SIZE>>U32_SIZE_LOG2)
#define LMD6_U64_COUNT (LMD6_SIZE>>U64_SIZE_LOG2)
