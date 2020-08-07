echo NOTE: Ignore error messages about \"cannot remove X\"
rm bin/getlmd
gcc -D_32_ -D_FILE_OFFSET_BITS=64 -O3 -fno-stack-protector -mtune=native -obin/getlmd getlmd.c
