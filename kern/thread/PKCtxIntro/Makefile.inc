# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PKCtxIntro

KERN_SRCFILES	+= $(KERN_DIR)/thread/PKCtxIntro/PKCtxIntro.c
KERN_SRCFILES	+= $(KERN_DIR)/thread/PKCtxIntro/cswitch.S

$(KERN_OBJDIR)/thread/PKCtxIntro/%.o: $(KERN_DIR)/thread/PKCtxIntro/%.c
	@echo + $(COMP_NAME)[KERN/thread/PKCtxIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PKCtxIntro/%.o: $(KERN_DIR)/thread/PKCtxIntro/%.S
	@echo + as[KERN/thread/PKCtxIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
