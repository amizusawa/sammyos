
symbol-file sammyos.bin
set arch i386
target remote localhost:1234
b kernel_main
b kpanic
