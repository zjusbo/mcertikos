# -*-Makefile-*-

OBJDIRS         += $(KERN_OBJDIR)/pcpu/PCPUInit

KERN_SRCFILES   += $(KERN_DIR)/pcpu/PCPUInit/PCPUInit.c

$(KERN_OBJDIR)/pcpu/PCPUInit/%.o: $(KERN_DIR)/pcpu/PCPUInit/%.c
	@echo + $(COMP_NAME)[KERN/pcpu/PCPUInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pcpu/PCPUInit/%.o: $(KERN_DIR)/pcpu/PCPUInit/%.S
	@echo + as[KERN/pcpu/PCPUInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<

