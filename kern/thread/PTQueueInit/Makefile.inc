# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PTQueueInit

KERN_SRCFILES	+= $(KERN_DIR)/thread/PTQueueInit/PTQueueInit.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/thread/PTQueueInit/test.c
endif

$(KERN_OBJDIR)/thread/PTQueueInit/%.o: $(KERN_DIR)/thread/PTQueueInit/%.c
	@echo + $(COMP_NAME)[KERN/thread/PTQueueInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PTQueueInit/%.o: $(KERN_DIR)/thread/PTQueueInit/%.S
	@echo + as[KERN/thread/PTQueueInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
