# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/pmm/MATOp

KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATOp/MATOp.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATOp/test.c
endif

$(KERN_OBJDIR)/pmm/MATOp/%.o: $(KERN_DIR)/pmm/MATOp/%.c
	@echo + $(COMP_NAME)[KERN/pmm/MATOp] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pmm/MATOp/%.o: $(KERN_DIR)/pmm/MATOp/%.S
	@echo + as[KERN/pmm/MATOp] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
