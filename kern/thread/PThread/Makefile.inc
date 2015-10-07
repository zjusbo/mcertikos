# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PThread

KERN_SRCFILES	+= $(KERN_DIR)/thread/PThread/PThread.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/thread/PThread/test.c
endif

$(KERN_OBJDIR)/thread/PThread/%.o: $(KERN_DIR)/thread/PThread/%.c
	@echo + $(COMP_NAME)[KERN/thread/PThread] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PThread/%.o: $(KERN_DIR)/thread/PThread/%.S
	@echo + as[KERN/thread/PThread] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
