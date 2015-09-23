# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/vmm/MPTNew

KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTNew/MPTNew.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTNew/test.c
endif

$(KERN_OBJDIR)/vmm/MPTNew/%.o: $(KERN_DIR)/vmm/MPTNew/%.c
	@echo + $(COMP_NAME)[KERN/vmm/MPTNew] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/vmm/MPTNew/%.o: $(KERN_DIR)/vmm/MPTNew/%.S
	@echo + as[KERN/vmm/MPTNew] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
