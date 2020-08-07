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

  LMD5_A0==(2^0x100-2^0xF3-2^0xEC-2^0xC2)
  LMD5_A1==(2^0x100-2^0xDC-2^0xCE-2^0xB7)

These numbers have been selected because they are easy to multiply by, and:

  LMD5_A0*2^0x100-1 is prime,
  LMD5_A1*2^0x100-1 is prime,
  LMD5_A0*2^0xFF-1 is prime,
  and LMD5_A1*2^0xFF-1 is prime.

which implies that the cycle lengths of the iterations (x,c)-->((LMD5_A0*x+c) MOD 2^0x200) and (x,c)-->((LMD5_A1*x+c) MOD 2^0x200) are the last 2 numbers above, respectively.

The exponents above are defined as follows:
*/
#define LMD5_A0_SHIFT_0 0xF3
#define LMD5_A0_SHIFT_1 0xEC
#define LMD5_A0_SHIFT_2 0xC2
#define LMD5_A1_SHIFT_0 0xDC
#define LMD5_A1_SHIFT_1 0xCE
#define LMD5_A1_SHIFT_2 0xB7
#define LMD5_BIT_COUNT (1<<(LMD5_SIZE_LOG2+U8_BITS_LOG2))
#define LMD5_BLOCK_SIZE 0x1000
#define LMD5_BLOCK_U32_COUNT (LMD5_BLOCK_SIZE>>U32_SIZE_LOG2)
#define LMD5_BLOCK_U64_COUNT (LMD5_BLOCK_SIZE>>U64_SIZE_LOG2)
#define LMD5_SIZE (1<<LMD5_SIZE_LOG2)
#define LMD5_SIZE_LOG2 6
#define LMD5_U32_COUNT (LMD5_SIZE>>U32_SIZE_LOG2)
#define LMD5_U64_COUNT (LMD5_SIZE>>U64_SIZE_LOG2)
