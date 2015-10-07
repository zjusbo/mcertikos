# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/trap/TSyscallArg

KERN_SRCFILES	+= $(KERN_DIR)/trap/TSyscallArg/TSyscallArg.c

$(KERN_OBJDIR)/trap/TSyscallArg/%.o: $(KERN_DIR)/trap/TSyscallArg/%.c
	@echo + $(COMP_NAME)[KERN/trap/TSyscallArg] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/trap/TSyscallArg/%.o: $(KERN_DIR)/trap/TSyscallArg/%.S
	@echo + as[KERN/trap/TSyscallArg] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
