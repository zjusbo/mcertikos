# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/idle

USER_IDLE_SRC	+= $(USER_DIR)/idle/idle.c
USER_IDLE_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_IDLE_SRC))
USER_IDLE_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_IDLE_OBJ))

KERN_BINFILES	+= $(USER_OBJDIR)/idle/idle

idle: $(USER_OBJDIR)/idle/idle

$(USER_OBJDIR)/idle/idle: $(USER_LIB_OBJ) $(USER_IDLE_OBJ)
	@echo + ld[USER/idle] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_IDLE_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/idle/%.o: $(USER_DIR)/idle/%.c
	@echo + cc[USER/idle] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/idle/%.o: $(USER_DIR)/idle/%.S
	@echo + as[USER/idle] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
