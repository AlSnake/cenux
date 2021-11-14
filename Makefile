HOSTARCH = i386
CC = i686-elf-gcc
LD = i686-elf-ld
NASM = nasm

CFLAGS  := -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -O0 -falign-jumps -falign-functions -falign-labels -fomit-frame-pointer -finline-functions -falign-loops -fstrength-reduce -Wno-unused-function -Wno-unused-parameter -fno-builtin -Wno-unused-label -Wno-cpp -Iinc
LDFLAGS :=
LIBS 	:= -lk
INCLUDES = -I./include -I./include/libc

ARCHDIR = arch/$(HOSTARCH)
LIBDIR	= -L./libc

include $(ARCHDIR)/make.config

CFLAGS 	:= $(CFLAGS) $(KERNEL_ARCH_CFLAGS) $(LIBDIR)
LDFLAGS	:= $(LDFLAGS)$(KERNEL_ARCH_LDFLAGS)
LIBS	:= $(LIBS)	 $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/tty.o \

OBJS=\
$(KERNEL_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(KERNEL_OBJS) \
$(LIBS) \

.PHONY: all libc clean
.SUFFIXES: .o .c .asm

all: libc cenux.kernel

cenux.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(NASM) -f bin $(ARCHDIR)/boot.asm -o $(ARCHDIR)/boot.bin
	$(LD) -g -relocatable $(OBJS) -o $(ARCHDIR)/kernelfull.o
	$(CC) -T $(ARCHDIR)/linker.ld -o $(ARCHDIR)/kernel.bin $(CFLAGS) $(ARCHDIR)/kernelfull.o $(LIBS)

	rm -rf cenux.kernel
	dd if=$(ARCHDIR)/boot.bin >> cenux.kernel
	dd if=$(ARCHDIR)/kernel.bin >> cenux.kernel
	dd if=/dev/zero bs=512 count=100 >> cenux.kernel

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDES)

.asm.o:
	$(NASM) -f elf -g $< -o $@

libc:
	make -C libc

clean:
	rm -f cenux.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	rm -f $(OBJS:.o=.bin) *.bin */*.bin */*/*.bin
	make -C libc clean

run:
	qemu-system-i386 -hda cenux.kernel

clangformat:
	clang-format -i *.c */*.c */*/*.c
	clang-format -i *.h */*.h */*/*.h

-include $(OBJS:.o=.d)
