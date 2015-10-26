# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/trap/TTrapInit

KERN_SRCFILES	+= $(KERN_DIR)/trap/TTrapInit/TTrapInit.c

$(KERN_OBJDIR)/trap/TTrapInit/%.o: $(KERN_DIR)/trap/TTrapInit/%.c
	@echo + $(COMP_NAME)[KERN/trap/TTrapInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/trap/TTrapInit/%.o: $(KERN_DIR)/trap/TTrapInit/%.S
	@echo + as[KERN/trap/TTrapInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
