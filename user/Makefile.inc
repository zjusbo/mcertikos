
USER_TOP := user

USER_CFLAGS	+= $(CFLAGS) -m32 -D_USER_ -O2 -I$(USER_TOP)/include
USER_LDFLAGS	:= $(LDFLAGS) -m elf_i386 -T$(USER_TOP)/linker.x -e _start

USER_GENDIR := $(OBJDIR)/gen

# Sub-makefiles
include $(USER_TOP)/config.mk
include $(USER_TOP)/lib/Makefile.inc
include $(USER_TOP)/proc/Makefile.inc

gen: $(USER_GENDIR)/user_procs.inc
	@echo "All targets of gen are done."

