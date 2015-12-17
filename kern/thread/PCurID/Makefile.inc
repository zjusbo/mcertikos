# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/thread/PCurID

KERN_SRCFILES	+= $(KERN_DIR)/thread/PCurID/PCurID.c

$(KERN_OBJDIR)/thread/PCurID/%.o: $(KERN_DIR)/thread/PCurID/%.c
	@echo + $(COMP_NAME)[KERN/thread/PCurID] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/thread/PCurID/%.o: $(KERN_DIR)/thread/PCurID/%.S
	@echo + as[KERN/thread/PCurID] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
