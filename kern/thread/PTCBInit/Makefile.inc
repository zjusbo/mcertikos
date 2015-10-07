# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PTCBInit

KERN_SRCFILES	+= $(KERN_DIR)/thread/PTCBInit/PTCBInit.c
ifdef TEST
KERN_SRCFILES	+= $(KERN_DIR)/thread/PTCBInit/test.c
endif

$(KERN_OBJDIR)/thread/PTCBInit/%.o: $(KERN_DIR)/thread/PTCBInit/%.c
	@echo + $(COMP_NAME)[KERN/thread/PTCBInit] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PTCBInit/%.o: $(KERN_DIR)/thread/PTCBInit/%.S
	@echo + as[KERN/thread/PTCBInit] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
