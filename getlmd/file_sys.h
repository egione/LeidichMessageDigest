/*
GetLMD File System Constants
http://leidich-message-digest.blogspot.com
Copyright 2012 Russell Leidich
September 20, 2012

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

#ifdef WINDOWS
  #define FSEEKO fseeko64
  #define FTELLO ftello64
#else
  #define FSEEKO fseeko
  #define FTELLO ftello
#endif

enum file_mode{
  FILE_READ,
  FILE_WRITE,
  FILE_APPEND
};
