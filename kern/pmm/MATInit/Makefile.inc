# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/pmm/MATInit

KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATInit/MATInit.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATInit/test.c
endif

$(KERN_OBJDIR)/pmm/MATInit/%.o: $(KERN_DIR)/pmm/MATInit/%.c
	@echo + $(COMP_NAME)[KERN/pmm/MATInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pmm/MATInit/%.o: $(KERN_DIR)/pmm/MATInit/%.S
	@echo + as[KERN/pmm/MATInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
