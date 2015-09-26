# -*-Makefile-*-

BOOT1_OBJFILES += $(OBJDIR)/boot/boot1.o
BOOT1_OBJFILES += $(OBJDIR)/boot/boot1lib.o
BOOT1_OBJFILES += $(OBJDIR)/boot/boot1main.o
BOOT1_OBJFILES += $(OBJDIR)/boot/exec_kernel.o

$(OBJDIR)/boot/boot1: $(BOOT1_OBJFILES) $(BOOT_LIB_OBJFILES)
	@echo + ld[BOOT] $@
	$(V)$(LD) $(BOOT_LDFLAGS) -N -e start -Ttext 0x7e00 -o $@.elf $^ $(GCC_LIB32)
	$(V)$(OBJDUMP) -z -D -S $@.elf > $@.asm
	$(V)$(OBJCOPY) -S -O binary $@.elf $@
	$(V)$(NM) -n $@.elf > $@.sym
	$(V)$(PERL) $(UTILSDIR)/check_size.pl 31744 $@

$(OBJDIR)/boot/%.o: boot/boot1/%.c
	@echo + cc[BOOT] $<
	@mkdir -p $(@D)
	$(V)$(CC) -nostdinc $(BOOT_CFLAGS) -Iboot/boot1 -Os -c -o $@ $<

$(OBJDIR)/boot/%.o: boot/boot1/%.S
	@echo + as[BOOT] $<
	@mkdir -p $(@D)
	$(V)$(CC) -nostdinc $(BOOT_CFLAGS) -Os -c -o $@ $<
