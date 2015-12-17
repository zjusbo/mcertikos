# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/vmm/MPTInit

KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTInit/MPTInit.c

$(KERN_OBJDIR)/vmm/MPTInit/%.o: $(KERN_DIR)/vmm/MPTInit/%.c
	@echo + $(COMP_NAME)[KERN/vmm/MPTInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/vmm/MPTInit/%.o: $(KERN_DIR)/vmm/MPTInit/%.S
	@echo + as[KERN/vmm/MPTInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
