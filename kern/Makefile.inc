# -*-Makefile-*-

KERN_DIR	:= kern
KERN_OBJDIR	:= $(OBJDIR)/kern

OBJDIRS		+= $(KERN_OBJDIR)

DEBUG_MSG = 1

# Arch-independent compiling and linking options
KERN_CFLAGS	:= $(CFLAGS) -D_KERN_ -I$(KERN_DIR) -I$(KERN_DIR)/kern -I. -m32
ifdef ENABLE_CCOMP
CCOMP_KERN_CFLAGS := $(CCOMP_CFLAGS) -D_KERN_ -I$(KERN_DIR) -I$(KERN_DIR)/kern -I.
CLIGHTGEN_FLAGS   += -D_KERN_ -I$(KERN_DIR) -I$(KERN_DIR)/kern -I.
endif

ifdef DEBUG_MSG
KERN_CFLAGS	+= -Og -g
else
KERN_CFLAGS	+= -O2
endif

KERN_LDFLAGS	:= $(LDFLAGS) -e start -m elf_i386 -Ttext=0x00100000
GCC_LIBS        := $(GCC_LIB32)

# Sub-makefiles
include		$(KERN_DIR)/config.mk
include		$(KERN_DIR)/dev/Makefile.inc
include		$(KERN_DIR)/lib/Makefile.inc
include		$(KERN_DIR)/init/Makefile.inc
include		$(KERN_DIR)/pmm/Makefile.inc
include		$(KERN_DIR)/vmm/Makefile.inc
include		$(KERN_DIR)/thread/Makefile.inc
include		$(KERN_DIR)/proc/Makefile.inc
include		$(KERN_DIR)/trap/Makefile.inc

KERN_CFLAGS	+= $(KERN_DEBUG_FLAGS)
KERN_CFLAGS	+= -DSERIAL_DEBUG -DDEBUG_MSG
ifdef ENABLE_CCOMP
CCOMP_KERN_CFLAGS += $(KERN_DEBUG_FLAGS)
CLIGHTGEN_FLAGS	  += $(KERN_DEBUG_FLAGS)
endif

# Object files of kernel
KERN_SRCFILES	:= $(wildcard $(KERN_SRCFILES))
KERN_OBJFILES	:= $(patsubst %.c, $(OBJDIR)/%.o, $(KERN_SRCFILES))
KERN_OBJFILES	:= $(patsubst %.S, $(OBJDIR)/%.o, $(KERN_OBJFILES))

# Targets

kern: $(KERN_OBJFILES) $(KERN_BINFILES)
	@echo All targets of kernel are compiled.

link: $(KERN_OBJDIR)/kernel

# Link kernel
$(KERN_OBJDIR)/kernel: $(KERN_OBJFILES) $(KERN_BINFILES) 
	@echo + ld[KERN] $@
	$(V)$(LD) -o $@ $(KERN_LDFLAGS) $(KERN_OBJFILES) $(GCC_LIBS) -b binary $(KERN_BINFILES)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

install_kern: $(KERN_OBJDIR)/kernel $(CERTIKOS_IMG)
	@echo + copy kernel to /boot/kernel
	$(V)$(UTILSDIR)/mount.sh
	$(V)sudo mkdir -p /mnt/boot
	$(V)sudo cp -f $(KERN_OBJDIR)/kernel /mnt/boot/kernel
	$(V)$(UTILSDIR)/umount.sh

kern_coq: $(KERN_CCOMP_SRC)
	@echo + clightgen $(KERN_CCOMP_SRC)
	$(V)$(CLIGHTGEN) $(CLIGHTGEN_FLAGS) $(KERN_CCOMP_SRC)

