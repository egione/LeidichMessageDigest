echo NOTE: Ignore error messages about \"cannot remove X\"
rm bin/getlmd
gcc -D_64_ -O3 -fno-stack-protector -mtune=native -obin/getlmd getlmd.c
