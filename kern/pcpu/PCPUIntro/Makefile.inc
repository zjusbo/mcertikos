# -*-Makefile-*-

OBJDIRS         += $(KERN_OBJDIR)/pcpu/PCPUIntro

KERN_SRCFILES   += $(KERN_DIR)/pcpu/PCPUIntro/PCPUIntro.c

$(KERN_OBJDIR)/pcpu/PCPUIntro/%.o: $(KERN_DIR)/pcpu/PCPUIntro/%.c
	@echo + $(COMP_NAME)[KERN/pcpu/PCPUIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pcpu/PCPUIntro/%.o: $(KERN_DIR)/pcpu/PCPUIntro/%.S
	@echo + as[KERN/pcpu/PCPUIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<

