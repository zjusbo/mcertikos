#
# Top-level Makefile for certikos64
#

ifndef V
V		:= @
else
V		:=
endif

ARCH		:= i386

COMP_NAME	:= "ccomp"

ENABLE_CCOMP	:= 0
COMP_NAME		:= "cc"

# Cross-compiler toolchain
#
# This Makefile will automatically use a cross-compiler toolchain installed
# as 'pios-*' or 'i386-elf-*', if one exists.  If the host tools ('gcc',
# 'objdump', and so forth) compile for a 32-bit x86 ELF target, that will
# be detected as well.  If you have the right compiler toolchain installed
# using a different name, set GCCPREFIX explicitly in conf/env.mk

# try to infer the correct GCCPREFIX
ifndef GCCPREFIX
GCCPREFIX := $(shell sh misc/gccprefix.sh)
endif

# Directories
TOP		:= .
SRCDIR		:= $(TOP)
OBJDIR		:= $(TOP)/obj
UTILSDIR	:= $(TOP)/misc
TESTDIR		:= $(TOP)/test
OBJDIRS		:=

# Compiler and Linker
CC		:= $(GCCPREFIX)gcc
LD		:= $(GCCPREFIX)ld
CFLAGS		:= -MD -Wno-strict-aliasing -Wno-unused-function -pipe -fno-builtin -nostdinc -fno-stack-protector
LDFLAGS		:= -nostdlib

ifeq (ENABLE_CCOMP, 1)
CCOMP		:= ccomp
CCOMP_CFLAGS	:= -finline-asm -fpacked-structs -fno-sse

CLIGHTGEN	:= clightgen
CLIGHTGEN_FLAGS	:= -fall

ifeq ($(V),)
CCOMP_CFLAGS	+= -v
endif

else
# Uncomment following two lines when you suspect differences between gcc and
# compcert cause problems.

CCOMP		:= $(GCCPREFIX)gcc
CCOMP_CFLAGS	:= -MD -Wno-strict-aliasing -Wno-unused-function -pipe -fno-builtin -nostdinc -fno-stack-protector -O2 -g -m32 -D__COMPCERT__
endif

# other tools
PERL		:= perl
OBJDUMP		:= $(GCCPREFIX)objdump
OBJCOPY		:= $(GCCPREFIX)objcopy
DD		:= $(GCCPREFIX)dd
NM		:= $(GCCPREFIX)nm
CSCOPE		:= cscope

# others
GCC_LIB32 := $(shell $(CC) $(CFLAGS) -m32 -print-libgcc-file-name)
ifeq ($(ARCH), amd64)
GCC_LIB64 := $(shell $(CC) $(CFLAGS) -m64 -print-libgcc-file-name)
endif

# If this is the first time building CertiKOS64, please follow the instructions
# in HOW_TO_MAKE_DISK_IMAGE to create a disk image file manually and put it in
# directory $(OBJDIR)/ (default: obj/)
CERTIKOS_IMG	:= certikos.img

# bochs
BOCHS		:= bochs
BOCHS_OPT	:= -q

# try to generate a unique GDB port
GDBPORT	:= $(shell expr `id -u` % 5000 + 25000)
 
# qemu
QEMU		:= qemu-system-x86_64
QEMUOPTS	:= -smp 1 -hda $(CERTIKOS_IMG) -serial mon:stdio -gdb tcp::$(GDBPORT) -m 2048 -k en-us
QEMUOPTS_KVM	:= -cpu host -enable-kvm
QEMUOPTS_BIOS	:= -L $(UTILSDIR)/qemu/

# Targets

.PHONY: all boot kern user deps qemu qemu-nox qemu-gdb


all: boot kern user link
	@./make_image.py
ifdef TEST
	@echo "***"
	@echo "*** Use Ctrl-a x to exit qemu"
	@echo "***"
	$(V)$(QEMU) -nographic $(QEMUOPTS)
endif
	@echo "All targets are done."

install_img: install_boot install_sys install_user
	@echo "CertiKOS is installed on the disk image."

bochs: $(CERTIKOS_IMG) .bochsrc
	@echo + start bochs
	$(V)$(BOCHS) $(BOCHS_OPT)

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@
	@./scripts/set_auto_load.sh

#	QEMU doesn't truncate the pcap file.  Work around this.
pre-qemu: .gdbinit
	@rm -f qemu.pcap

qemu: $(CERTIKOS_IMG) pre-qemu
	$(V)$(QEMU) $(QEMUOPTS)

qemu-nox: $(CERTIKOS_IMG) pre-qemu
	@echo "***"
	@echo "*** Use Ctrl-a x to exit qemu"
	@echo "***"
	$(V)$(QEMU) -nographic $(QEMUOPTS)

qemu-gdb: $(CERTIKOS_IMG) pre-qemu
	@echo "***"
	@echo "*** Now run 'gdb'." 1>&2
	@echo "***"
	$(V)$(QEMU) $(QEMUOPTS) -S

qemu-nox-gdb: $(CERTIKOS_IMG) pre-qemu
	@echo "***"
	@echo "*** Now run 'gdb'." 1>&2
	@echo "***"
	$(V)$(QEMU) -nographic $(QEMUOPTS) -S

gdb: pre-qemu
	@gdb

qemu-kvm: $(CERTIKOS_IMG)
	$(V)$(QEMU) $(QEMUOPTS) $(QEMUOPTS_KVM)

qemu-bios: $(CERTIKOS_IMG)
	$(V)$(QEMU) $(QEMUOPTS) $(QEMUOPTS_BIOS)

iso: all
	$(V)cp $(OBJDIR)/kern/kernel $(UTILSDIR)/iso/boot/kernel
	$(V)grub-mkrescue -o $(CERTIKOS_IMG) $(UTILSDIR)/iso

package:
	$(V)tar czf ../certikos.tar.gz --exclude=obj --exclude=cscope.* .

cscope:
	$(V)rm -rf cscope.*
	$(V)find . -name "*.[chsS]" > cscope.files
	$(V)cscope -bkq -i cscope.files


# Sub-makefiles
include boot/Makefile.inc
include user/Makefile.inc
include kern/Makefile.inc

deps: $(OBJDIR)/.deps

$(OBJDIR)/.deps: $(foreach dir, $(OBJDIRS), $(wildcard $(dir)/*.d))
	$(V)mkdir -p $(@D)
	$(V)$(PERL) $(UTILSDIR)/mergedep.pl $@ $^

-include $(OBJDIR)/.deps

clean:
	$(V)rm -rf $(OBJDIR) .gdbinit certikos.img
	$(V)find . -name "*.[v]" -delete
