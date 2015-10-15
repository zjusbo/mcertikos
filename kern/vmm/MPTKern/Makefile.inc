# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/vmm/MPTKern

KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTKern/MPTKern.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTKern/test.c
endif

$(KERN_OBJDIR)/vmm/MPTKern/%.o: $(KERN_DIR)/vmm/MPTKern/%.c
	@echo + $(COMP_NAME)[KERN/vmm/MPTKern] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/vmm/MPTKern/%.o: $(KERN_DIR)/vmm/MPTKern/%.S
	@echo + as[KERN/vmm/MPTKern] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
