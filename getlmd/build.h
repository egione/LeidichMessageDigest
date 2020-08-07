/*
GetLMD Build Control
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
/*
Build control. If possible, change the build using gcc command switches, and not by changing this file.
*/
#define BUILD_NUMBER 64
#define expect_false(x) __builtin_expect((x),0)
#define expect_true(x) __builtin_expect((x),1)

#if !(defined(_32_)||defined(_64_))
#error "Use \"gcc -D_64_\" for 64-bit or \"gcc -D_32_\" for 32-bit code."
#elif defined(_32_)&&defined(_64_)
#error "You have defined both _32_ and _64_. Chose one only."
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef WINDOWS
#undef WINDOWS
#endif

#if defined(_WIN32)||defined(__WIN32__)||defined(_WIN64)||defined(__WIN64__)
#define WINDOWS
#endif
