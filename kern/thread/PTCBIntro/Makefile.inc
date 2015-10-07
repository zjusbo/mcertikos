# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PTCBIntro

KERN_SRCFILES	+= $(KERN_DIR)/thread/PTCBIntro/PTCBIntro.c

$(KERN_OBJDIR)/thread/PTCBIntro/%.o: $(KERN_DIR)/thread/PTCBIntro/%.c
	@echo + $(COMP_NAME)[KERN/thread/PTCBIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PTCBIntro/%.o: $(KERN_DIR)/thread/PTCBIntro/%.S
	@echo + as[KERN/thread/PTCBIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
