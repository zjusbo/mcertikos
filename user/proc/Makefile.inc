# -*-Makefile-*-

USER_DIR	:= $(USER_TOP)/proc
USER_OBJDIR	:= $(OBJDIR)/proc

EXTERNAL_BINFILES_DIR := $(OBJDIR)/external_binfiles


OBJDIRS		+= $(USER_OBJDIR)

USER_BINFILES :=

USER_MAKEFILES := $(wildcard $(USER_DIR)/*/Makefile.inc)

EXTERNAL_BINFILES_FULL := $(patsubst %, $(EXTERNAL_BINFILES_DIR)/%, $(EXTERNAL_BINFILES))

include $(USER_MAKEFILES)

user_procs: $(USER_BINFILES) $(USER_MAKEFILES) $(USER_GENDIR)/user_procs.h
	@echo All targets of proc are done.

$(USER_GENDIR)/user_procs.h: $(USER_MAKEFILES)
	@echo + py[gen/user_procs.h] $@
	$(V)mkdir -p $(OBJDIR)/gen
	$(V)mkdir -p $(EXTERNAL_BINFILES_DIR)
	$(V)python misc/build_user_proc_table.py $(EXTERNAL_BINFILES_FULL) \
	$(USER_BINFILES) > $(USER_GENDIR)/user_procs.h

$(USER_GENDIR)/user_procs.inc: $(USER_MAKEFILES)
	@echo + ec[gen/user_procs.inc] $@
	$(V)mkdir -p $(OBJDIR)/gen
	$(V)echo $(EXTERNAL_BINFILES_FULL) $(USER_BINFILES) > \
	$(USER_GENDIR)/user_procs.inc

$(USER_MAKEFILES):

install_user:
