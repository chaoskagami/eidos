 # Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

CONF=-DCONF_EARLY_KPRINTF=1

TGT=i686

SOURCES= \
    $(TGT)/entry.o \
    $(TGT)/utils.o \
    module/biosvga/module.o module/serial/module.o module/console.o \
    common/stdc/memmove.o common/stdc/strlen.o common/stdc/memcmp.o common/stdc/memset.o common/stdc/memcpy.o \
    common/kernel_main.o

LDSC=$(TGT)/link.ld

TGTNAM=$(TGT)-elf
CC=$(TGTNAM)-gcc
AS=$(TGTNAM)-as
CXX=$(TGTNAM)-g++
LD=$(TGTNAM)-ld
OC=$(TGTNAM)-objcopy
NM=$(TGTNAM)-nm
RL=$(TGTNAM)-ranlib

CFLAGS=-ffreestanding -std=gnu11
CPPFLAGS=-Iinclude -I$(TGT)/include
LDFLAGS=-T$(LDSC)
ASFLAGS=
LIBS=

all: link

qemu: all
	qemu-system-i386 --kernel kernel

clean:
	rm -f $(SOURCES) kernel

link: $(SOURCES)
	$(LD) $(LDFLAGS) -o kernel $(LIBS) $(SOURCES)

%.o: %.c
	$(CC) $(CFLAGS) $(CONF) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CONF) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<

%.o: %.S
	$(AS) $(ASFLAGS) -c -o $@ $<
