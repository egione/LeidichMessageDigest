/*
Leidich Message Digest Library
http://leidich-message-digest.blogspot.com
Copyright 2013-2014 Russell Leidich
May 19, 2014

This collection of files constitutes the Leidich Message Digest Library. (This
is a library in the abstact sense; it's not intended to compile to a ".lib"
file.)

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
Getting Started:

  See notes under linux_build.sh or win_build.bat below.

Files included:

  COPYING: Licensing info.

  demo.c: Demo code for LMD.

  linux_build.sh: is for building on any flavor of GCC under 32/64-bit
  Linux. It will build a file called "demo". (You may see an error message
  about "cannot remove demo", because it tries to delete any previous
  instance before compiling a new one.) You can then run "./demo" to see a
  rather boring demonstration. Note that you may need to run this via
  "/bin/sh ./linux_build.sh".

  lmd2.c: Leidich Message Digest 2 and 3.

  lmd2.h: Leidich Message Digest 2 and 3 include file.

  lmd4.c: Leidich Message Digest 4.

  lmd4.h: Leidich Message Digest 4 include file.

  lmd5.c: Leidich Message Digest 5.

  lmd5.h: Leidich Message Digest 5 include file.

  lmd6_32.c: Leidich Message Digest 6 optimized for 32-bit architecture
  (also works on 64-bit).

  lmd6_64.c: Leidich Message Digest 6 optimized for 64-bit architecture
  (also works on 32-bit).

  lmd6.h: Leidich Message Digest 6 include file.

  lmd7.c: Leidich Message Digest 7.

  lmd7.h: Leidich Message Digest 7 include file.

  mathematica.txt: Examples based on Mathematica Kernel.

  win_build.bat: Build batch file strictly for the MinGW build environment
  under Windows. You must first:

      SET TEMP=(Path for temporary files, e.g. C:\WINDOWS\Temp)

    Then you can run "%TEMP%\demo.exe".

    Otherwise, have a look at the compiler invokation in linux_build.bat,
    and adapt it to your own build environment.
