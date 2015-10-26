# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/init

KERN_SRCFILES	+= $(KERN_DIR)/init/init.c
KERN_SRCFILES	+= $(KERN_DIR)/init/entry.S

KERN_BINFILES	+= $(KERN_OBJDIR)/init/boot_ap

$(KERN_OBJDIR)/init/%.o: $(KERN_DIR)/init/%.c
	@echo + $(COMP_NAME)[KERN/init] $<
	@mkdir -p $(@D)
	$(V)$(CCOMP) $(CCOMP_KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/init/%.o: $(KERN_DIR)/init/%.S
	@echo + as[KERN/init] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<


$(KERN_OBJDIR)/init/boot_ap.o: $(KERN_DIR)/init/boot_ap.S
	@echo + as[KERN/init] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<


$(KERN_OBJDIR)/init/boot_ap: $(KERN_OBJDIR)/init/boot_ap.o
	@echo + ld[KERN/init] $<
	$(V)$(LD) -nostdlib -m elf_i386 -e start_ap -N -Ttext 0x8000 -o $@.elf $^
	$(V)$(OBJDUMP) -S $@.elf >$@.asm
	$(V)$(OBJCOPY) -S -O binary $@.elf $@
