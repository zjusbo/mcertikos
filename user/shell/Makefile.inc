# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/shell

USER_SHELL_SRC	+= $(USER_DIR)/shell/shell.c
USER_SHELL_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_SHELL_SRC))
USER_SHELL_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_SHELL_OBJ))

KERN_BINFILES	+= $(USER_OBJDIR)/shell/shell

shell: $(USER_OBJDIR)/shell/shell

$(USER_OBJDIR)/shell/shell: $(USER_LIB_OBJ) $(USER_SHELL_OBJ)
	@echo + ld[USER/shell] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_SHELL_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/shell/%.o: $(USER_DIR)/shell/%.c
	@echo + cc[USER/shell] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/shell/%.o: $(USER_DIR)/shell/%.S
	@echo + as[USER/shell] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
