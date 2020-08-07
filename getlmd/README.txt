/*
GetLMD Library
http://leidich-message-digest.blogspot.com
Copyright 2013-2017 Russell Leidich
February 26, 2017

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

USAGE:

  Computes the LMD/LMD2/LMD3/LMD7/LMD8/Checksum of a file or directory.

  Executables reside in the bin directory: getlmd32 (X86 Linux), getlmd64
  (X64 Linux), or getlmd(.exe) (Windows). Or build it yourself with the
  scripts described below, which is necessary for Mac.

  Exact syntax will be provided by the executable itself.

  The code isn't beautiful or well commented, but reliable execution
  can be verified by comparing results across different platforms.
  Files larger than 4GB should evaluate correctly, as should null files.

  Metadata such as the file name, permissions, and timestamp are NOT
  hashed.

ERRATA:

  If you've made the mistake of using a file system with UTF-16
  (as opposed to UTF-8 or ASCII) filenames, as sometimes happens
  in Windows but probably never in Linux, then you may on rare
  occasion encounter files which GetLMD assumes to be inaccessible
  due to the presence of null (zero) bytes within the filename.
  The null is historically a string terminator, but in UTF-16 it
  can be the middle of a non-ASCII character. At least, GetLMD
  will alert you with an error message.

FILES INCLUDED:

  bin/getlmd32: X86 Linux executable.

  bin/getlmd64: X64 Linux executable.

  bin/getlmd_win.exe: X86 Windows executable.

  build.h: Build control.

  constant.h: Documentary constants.

  COPYING: Licensing info.

  file_sys.h: File system constants.

  getlmd.c: Main program.

  linux32_build.sh: Build script for 32-bit Linux.

  linux64_build.sh: Build script for 64-bit Linux.

  mac64_build.sh: Build script for 64-bit Mac.

  lmd7.*: LMD7 secure hash.

  lmd8.*: LMD8 secure hash.

  win32_build.bat: Build batch file for 32-bit Windows.

INSTALLING IN LINUX:

  You can build it yourself as described below, or you can simply copy the
  precompiled executable to /usr/bin:

  sudo cp bin/getlmd(32/64) /usr/bin/getlmd

  Run it:

  getlmd

INSTALLING IN MAC:

  You must build it yourself as described below.

INSTALLING IN WINDOWS:

  You can build it yourself as described below, or you can simply copy the
  precompiled executable to c:\windows:

  copy /b bin\getlmd_win.exe c:\windows\getlmd.exe

  Run it:

  getlmd

BUILDING IN LINUX:

  /bin/sh ./linux(32/64)_build.sh

  This script deletes some build files, so read it first if you're paranoid.

  Compiles to bin/getlmd, which you can run with "bin/getlmd".

BUILDING IN MAC:

  /bin/sh ./mac64_build.sh

  This script deletes some build files, so read it first if you're paranoid.

  Compiles to bin/getlmd, which you can run with "bin/getlmd".

BUILDING IN WINDOWS:

  win32_build.bat is strictly for the MinGW build environment. 

  This batch file deletes some build files, so read it first if you're paranoid.

  Compiles to "bin\getlmd.exe".

  Installing MinGW:

  You need MinGW (http://mingw.org) for the CMD command shell. Cygwin is not supported. At mingw.org, download mingw-get-setup.exe (upper right "Download Installer" button). When you run it, select at least mingw32-base. From the "Installation" dropdown menu, select "Apply Changes". Click "Apply". When the installer is done, close it manually. Then, from the (Windows 7) Start menu, go to "Computer" -> right click -> Properties -> "Advanced system settings" -> Advanced -> "Environment Variables". In the "System variables" subwindow, select "Path" and click "Edit". Hit the "End" key. Append ";c:\mingw\bin" or wherever you have gcc.exe. (Note the semicolon!) Hit OK 3 times to close 3 windows. Close the Control Panel.

  Note that the MinGW-64 spinoff project has support for native X64 execution. It's not directly supported, but in principle it should work if you can hack our makefile.

  From the Start menu, enter "cmd" in the search/run dialog. Hit Enter. This should bring up a terminal. Change to the GetLMD folder: "cd \getlmd" or whatever.

  Run "win32_build.bat" at the command prompt. If an error dialog pops up about a missing DLL, then delete the entire c:\mingw folder and reinstall all of the above (but don't modify Path). There seems to be an intermittant file synchronization problem with the install process which omits a certain DLL, but only sometimes at random. Maybe leave the completed mingw-get-setup install process idle for a minute before closing it.
