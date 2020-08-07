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

  LMD4_A0==(2^0x80-2^0x7D-2^0x6E-2^0x64)
  LMD4_A1==(2^0x80-2^0x65-2^0x62-2^0x4C)

These numbers have been selected because they are easy to multiply by, and:

  LMD4_A0*2^0x80-1 is prime,
  LMD4_A1*2^0x80-1 is prime,
  LMD4_A0*2^0x7F-1 is prime,
  and LMD4_A1*2^0x7F-1 is prime.

which implies that the cycle lengths of the iterations (x,c)-->((LMD4_A0*x+c) MOD 2^0x100) and (x,c)-->((LMD4_A1*x+c) MOD 2^0x100) are the last 2 numbers above, respectively.

The exponents above are defined as follows:
*/
#define LMD4_A0_SHIFT_0 0x7D
#define LMD4_A0_SHIFT_1 0x6E
#define LMD4_A0_SHIFT_2 0x64
#define LMD4_A1_SHIFT_0 0x65
#define LMD4_A1_SHIFT_1 0x62
#define LMD4_A1_SHIFT_2 0x4C
#define LMD4_BIT_COUNT (1<<(LMD4_SIZE_LOG2+U8_BITS_LOG2))
#define LMD4_BLOCK_SIZE 0x1000
#define LMD4_BLOCK_U32_COUNT (LMD4_BLOCK_SIZE>>U32_SIZE_LOG2)
#define LMD4_BLOCK_U64_COUNT (LMD4_BLOCK_SIZE>>U64_SIZE_LOG2)
#define LMD4_SIZE (1<<LMD4_SIZE_LOG2)
#define LMD4_SIZE_LOG2 5
#define LMD4_U32_COUNT (LMD4_SIZE>>U32_SIZE_LOG2)
#define LMD4_U64_COUNT (LMD4_SIZE>>U64_SIZE_LOG2)
