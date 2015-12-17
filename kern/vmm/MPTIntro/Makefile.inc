# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/vmm/MPTIntro

KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTIntro/MPTIntro.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/vmm/MPTIntro/test.c
endif

$(KERN_OBJDIR)/vmm/MPTIntro/%.o: $(KERN_DIR)/vmm/MPTIntro/%.c
	@echo + $(COMP_NAME)[KERN/vmm/MPTIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/vmm/MPTIntro/%.o: $(KERN_DIR)/vmm/MPTIntro/%.S
	@echo + as[KERN/vmm/MPTIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
