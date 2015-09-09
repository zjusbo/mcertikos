# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/pmm/MATIntro

KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATIntro/MATIntro.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/pmm/MATIntro/test.c
endif

$(KERN_OBJDIR)/pmm/MATIntro/%.o: $(KERN_DIR)/pmm/MATIntro/%.c
	@echo + $(COMP_NAME)[KERN/pmm/MATIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/pmm/MATIntro/%.o: $(KERN_DIR)/pmm/MATIntro/%.S
	@echo + as[KERN/pmm/MATIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
