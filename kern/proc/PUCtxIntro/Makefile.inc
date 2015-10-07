# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/proc/PUCtxIntro

KERN_SRCFILES	+= $(KERN_DIR)/proc/PUCtxIntro/PUCtxIntro.c

$(KERN_OBJDIR)/proc/PUCtxIntro/%.o: $(KERN_DIR)/proc/PUCtxIntro/%.c
	@echo + $(COMP_NAME)[KERN/proc/PUCtxIntro] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/proc/PUCtxIntro/%.o: $(KERN_DIR)/proc/PUCtxIntro/%.S
	@echo + as[KERN/proc/PUCtxIntro] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
