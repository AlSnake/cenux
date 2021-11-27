set auto-load safe-path .
set disassembly-flavor intel
add-symbol-file ./boot/kernelfull.o 0x100000
break kernel.c:40
target remote | qemu-system-i386 -hda cenux.kernel -S -gdb stdio
