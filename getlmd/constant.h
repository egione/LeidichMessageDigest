/*
GetLMD Documentary Constants
http://leidich-message-digest.blogspot.com
Copyright 2012 Russell Leidich
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

/*
Stuff you can't change, and just adds documentation value.
*/
#define i16 int16_t
#define I16_BITS 16
#define I16_BITS_LOG2 4
#define I16_BYTE_MAX 1 
#define I16_MAX 0x7FFF
#define I16_MIN ((-I16_MAX)-1)
#define I16_SIZE 2
#define I16_SIZE_LOG2 1
#define I16_SPAN 0x10000
#define i32 int32_t
#define I32_BITS 32
#define I32_BITS_LOG2 5
#define I32_BYTE_MAX 3
#define I32_MAX 0x7FFFFFFF
#define I32_MIN ((-I32_MAX)-1)
#define I32_SIZE 4
#define I32_SIZE_LOG2 2
#define I32_SPAN 0x100000000ULL
#define i64 int64_t
#define I64_BITS 64
#define I64_BITS_LOG2 6
#define I64_BYTE_MAX 7
#define I64_MAX 0x7FFFFFFFFFFFFFFFLL
#define I64_MIN ((-I64_MAX)-1)
#define I64_SIZE 8
#define I64_SIZE_LOG2 3
#define i8 signed char
#define I8_BITS 8
#define I8_BITS_LOG2 3
#define I8_MAX 0x7F
#define I8_MIN ((-I8_MAX)-1)
#define I8_SIZE 1
#define I8_SIZE_LOG2 0
#define I8_SPAN 0x100
#define u16 uint16_t
#define U16_BIT_MAX 15
#define U16_BITS 16
#define U16_BITS_LOG2 4
#define U16_BYTE_MAX 1
#define U16_MAX 0xFFFF
#define U16_SIZE 2
#define U16_SIZE_LOG2 1
#define U16_SPAN 0x10000
#define U16_SPAN_HALF 0x8000UL
#define u32 uint32_t
#define U32_BIT_MAX 31
#define U32_BITS 32
#define U32_BITS_LOG2 5
#define U32_BYTE_MAX 3
#define U32_MAX 0xFFFFFFFFUL
#define U32_SIZE 4
#define U32_SIZE_LOG2 2
#define U32_SPAN 0x100000000ULL
#define U32_SPAN_HALF 0x80000000UL
#define u64 uint64_t
#define U64_BIT_MAX 63
#define U64_BITS 64
#define U64_BITS_LOG2 6
#define U64_BYTE_MAX 7
#define U64_MAX 0xFFFFFFFFFFFFFFFFULL
#define U64_SIZE 8
#define U64_SIZE_LOG2 3
#define U64_SPAN_HALF 0x8000000000000000ULL
#define u8 unsigned char
#define U8_BIT_MAX 7
#define U8_BITS 8
#define U8_BITS_LOG2 3
#define U8_MAX 0xFF
#define U8_SIZE 1
#define U8_SIZE_LOG2 0
#define U8_SPAN 0x100
#define U8_SPAN_HALF 0x80UL
#ifdef _64_
  #define ULONG uint64_t
  #define ULONG_MAX U64_MAX
  #define ULONG_SIZE 8
  #define ULONG_SIZE_LOG2 3
#else
  #define ULONG uint32_t
  #define ULONG_MAX U32_MAX
  #define ULONG_SIZE 4
  #define ULONG_SIZE_LOG2 2
#endif
