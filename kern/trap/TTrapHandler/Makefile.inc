# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/trap/TTrapHandler

KERN_SRCFILES	+= $(KERN_DIR)/trap/TTrapHandler/TTrapHandler.c

$(KERN_OBJDIR)/trap/TTrapHandler/%.o: $(KERN_DIR)/trap/TTrapHandler/%.c
	@echo + $(COMP_NAME)[KERN/trap/TTrapHandler] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/trap/TTrapHandler/%.o: $(KERN_DIR)/trap/TTrapHandler/%.S
	@echo + as[KERN/trap/TTrapHandler] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
