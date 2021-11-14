CC = i686-elf-gcc

CFLAGS  := -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -O0 -falign-jumps -falign-functions -falign-labels -fomit-frame-pointer -finline-functions -falign-loops -fstrength-reduce -Wno-unused-function -Wno-unused-parameter -fno-builtin -Wno-unused-label -Wno-cpp -Iinc 
NFLAGS	:= -g -O0
LIBK_CFLAGS:=$(CFLAGS)
INCLUDES = -I../include/libc
 
LIBCOBJS=\
string/memset.o \
string/strlen.o \
 
OBJS=\
$(LIBCOBJS) \
 
LIBK_OBJS=$(LIBCOBJS:.o=.libk.o)
 
#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a
 
.PHONY: all clean
.SUFFIXES: .o .libk.o .c .asm
 
all: $(BINARIES)
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDES)
 
.asm.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)
 
.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(LIBK_CFLAGS) $(INCLUDES)
 
.asm.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)
 
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d
  
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
