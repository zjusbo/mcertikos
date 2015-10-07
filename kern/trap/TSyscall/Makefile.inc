# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/trap/TSyscall

KERN_SRCFILES	+= $(KERN_DIR)/trap/TSyscall/TSyscall.c

$(KERN_OBJDIR)/trap/TSyscall/%.o: $(KERN_DIR)/trap/TSyscall/%.c
	@echo + $(COMP_NAME)[KERN/trap/TSyscall] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/trap/TSyscall/%.o: $(KERN_DIR)/trap/TSyscall/%.S
	@echo + as[KERN/trap/TSyscall] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
