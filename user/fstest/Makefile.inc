# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/fstest

USER_FSTEST_SRC	+= $(USER_DIR)/fstest/fstest.c
USER_FSTEST_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_FSTEST_SRC))
USER_FSTEST_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_FSTEST_OBJ))
KERN_BINFILES	+= $(USER_OBJDIR)/fstest/fstest

fstest: $(USER_OBJDIR)/fstest/fstest

$(USER_OBJDIR)/fstest/fstest: $(USER_LIB_OBJ) $(USER_FSTEST_OBJ)
	@echo + ld[USER/fstest] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_FSTEST_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/fstest/%.o: $(USER_DIR)/fstest/%.c
	@echo + cc[USER/fstest] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/fstest/%.o: $(USER_DIR)/fstest/%.S
	@echo + as[USER/fstest] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
