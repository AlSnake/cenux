set auto-load safe-path .
target remote | qemu-system-i386 -hda ./bin/boot.bin -S -gdb stdio
