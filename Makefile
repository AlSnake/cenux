CC = i686-elf-gcc
LD = i686-elf-ld
NASM = nasm

CFLAGS  := -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -O0 -falign-jumps -falign-functions -falign-labels -fomit-frame-pointer -finline-functions -falign-loops -fstrength-reduce -Wno-unused-function -Wno-unused-parameter -fno-builtin -Wno-unused-label -Wno-cpp -Iinc
LDFLAGS :=
LIBS 	:= 
INCLUDES = -I./include

LIBDIR	= 

BOOTDIR   = ./boot
KERNELDIR = ./kernel
CPUDIR	  = ./cpu
MMDIR	  = ./mm
DRIVERSDIR= ./drivers
FSDIR	  = ./fs

include $(BOOTDIR)/make.config
include $(KERNELDIR)/make.config
include $(CPUDIR)/make.config
include $(MMDIR)/make.config
include $(DRIVERSDIR)/make.config
include $(FSDIR)/make.config

CFLAGS 	:= $(CFLAGS) $(LIBDIR)

OBJS=\
$(BOOT_OBJS) \
$(KERNEL_OBJS) \
$(CPU_OBJS) \
$(MM_OBJS) \
$(DRIVERS_OBJS) \
$(FS_OBJS) \

.PHONY: all clean
.SUFFIXES: .o .c .asm.o .asm

all: cenux.bin

cenux.bin: $(OBJS) $(BOOTDIR)/linker.ld
	$(NASM) -f bin $(BOOTDIR)/boot.asm -o $(BOOTDIR)/boot.bin
	$(LD) -g -relocatable $(OBJS) -o $(BOOTDIR)/kernelfull.o
	$(CC) -T $(BOOTDIR)/linker.ld -o $(BOOTDIR)/kernel.bin $(CFLAGS) $(BOOTDIR)/kernelfull.o $(LIBS)

	rm -rf cenux.bin
	dd if=$(BOOTDIR)/boot.bin >> cenux.bin
	dd if=$(BOOTDIR)/kernel.bin >> cenux.bin
	dd if=/dev/zero bs=512 count=100 >> cenux.bin

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDES)

%.asm.o: %.asm
	$(NASM) -f elf -g $< -o $@

clean:
	rm -f cenux.bin
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	rm -f $(OBJS:.o=.bin) *.bin */*.bin */*/*.bin

run:
	qemu-system-i386 -hda cenux.bin

clangformat:
	clang-format -i *.c */*.c */*/*.c

-include $(OBJS:.o=.d)
