# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/proc/PProc

KERN_SRCFILES	+= $(KERN_DIR)/proc/PProc/PProc.c

$(KERN_OBJDIR)/proc/PProc/%.o: $(KERN_DIR)/proc/PProc/%.c
	@echo + $(COMP_NAME)[KERN/proc/PProc] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/proc/PProc/%.o: $(KERN_DIR)/proc/PProc/%.S
	@echo + as[KERN/proc/PProc] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
