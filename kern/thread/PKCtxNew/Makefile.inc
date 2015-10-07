# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PKCtxNew

KERN_SRCFILES	+= $(KERN_DIR)/thread/PKCtxNew/PKCtxNew.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/thread/PKCtxNew/test.c
endif

$(KERN_OBJDIR)/thread/PKCtxNew/%.o: $(KERN_DIR)/thread/PKCtxNew/%.c
	@echo + $(COMP_NAME)[KERN/thread/PKCtxNew] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PKCtxNew/%.o: $(KERN_DIR)/thread/PKCtxNew/%.S
	@echo + as[KERN/thread/PKCtxNew] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
