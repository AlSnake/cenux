set auto-load safe-path .

add-symbol-file ./boot/kernelfull.o 0x100000
break kmain
target remote | qemu-system-i386 -hda cenux.kernel -S -gdb stdio
