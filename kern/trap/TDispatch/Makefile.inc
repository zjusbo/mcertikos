# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/trap/TDispatch

KERN_SRCFILES	+= $(KERN_DIR)/trap/TDispatch/TDispatch.c

$(KERN_OBJDIR)/trap/TDispatch/%.o: $(KERN_DIR)/trap/TDispatch/%.c
	@echo + $(COMP_NAME)[KERN/trap/TDispatch] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/trap/TDispatch/%.o: $(KERN_DIR)/trap/TDispatch/%.S
	@echo + as[KERN/trap/TDispatch] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
