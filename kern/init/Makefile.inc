# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/init

KERN_SRCFILES	+= $(KERN_DIR)/init/init.c
KERN_SRCFILES	+= $(KERN_DIR)/init/entry.S

$(KERN_OBJDIR)/init/%.o: $(KERN_DIR)/init/%.c
	@echo + $(COMP_NAME)[KERN/init] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/init/%.o: $(KERN_DIR)/init/%.S
	@echo + as[KERN/init] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
