all:
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

run:
	qemu-system-i386 -hda bin/boot.bin

clean:
	rm -rf ./bin/boot.bin
	