# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PTQueueIntro

KERN_SRCFILES	+= $(KERN_DIR)/thread/PTQueueIntro/PTQueueIntro.c

$(KERN_OBJDIR)/thread/PTQueueIntro/%.o: $(KERN_DIR)/thread/PTQueueIntro/%.c
	@echo + $(COMP_NAME)[KERN/thread/PTQueueIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PTQueueIntro/%.o: $(KERN_DIR)/thread/PTQueueIntro/%.S
	@echo + as[KERN/thread/PTQueueIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
