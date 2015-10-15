# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/pmm/MContainer

KERN_SRCFILES	+= $(KERN_DIR)/pmm/MContainer/MContainer.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/pmm/MContainer/test.c
endif

$(KERN_OBJDIR)/pmm/MContainer/%.o: $(KERN_DIR)/pmm/MContainer/%.c
	@echo + $(COMP_NAME)[KERN/pmm/MContainer] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pmm/MContainer/%.o: $(KERN_DIR)/pmm/MContainer/%.S
	@echo + as[KERN/pmm/MContainer] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
