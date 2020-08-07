del bin\getlmd.exe
gcc -D_32_ -D_FILE_OFFSET_BITS=64 -O3 -fno-stack-protector -mtune=native -obin\getlmd.exe getlmd.c
