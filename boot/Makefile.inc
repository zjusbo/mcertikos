# -*-Makefile-*-

BOOT_DIR	:= $(TOP)/boot

# Compiling and Linking options for the bootloader
BOOT_CFLAGS	:= $(CFLAGS) -Os -g -m32 -I.
BOOT_LDFLAGS	:= $(LDFLAGS) -m elf_i386

BOOT_DEBUG	:= " "
BOOT_CONSOLE	:= " "

ifdef BOOT_DEBUG
BOOT_COMPILE_DEF += -DBOOT_DEBUG
endif

ifdef BOOT_CONSOLE
BOOT_COMPILE_DEF += -DBOOT_CONSOLE
endif

BOOT_CFLAGS	+= $(BOOT_COMPILE_DEF)

ifdef ENABLE_BOOT_CF
BOOT_CFLAGS	+= -DENABLE_BOOT_CF
endif

# Object files generated for the bootloader
BOOT_BINFILES	+= $(OBJDIR)/boot/boot0
BOOT_BINFILES	+= $(OBJDIR)/boot/boot1

# Sub-makefiles for the bootloader
include $(BOOT_DIR)/boot0/Makefile.inc
include $(BOOT_DIR)/boot1/Makefile.inc

# Top-level target to build the bootloader
boot: $(BOOT_BINFILES)
	@echo All targets of boot loader are done.

