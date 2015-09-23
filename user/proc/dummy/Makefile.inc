# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/dummy

USER_BINFILES	+= $(USER_OBJDIR)/dummy/dummy

USER_tests_SRC	+= $(wildcard $(USER_DIR)/dummy/*.c)
USER_tests_SRC	+= $(wildcard $(USER_DIR)/dummy/*.S)

USER_tests_dummy_OBJ	:= $(OBJDIR)/proc/dummy/dummy.o

$(USER_OBJDIR)/dummy/dummy: $(USER_LIB_OBJ) $(USER_tests_dummy_OBJ)
	@echo + ld[USER/dummy] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_tests_dummy_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym	

$(USER_OBJDIR)/dummy/%.o: $(USER_DIR)/dummy/%.c
	@echo + cc[USER/tests] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/dummy/%.o: $(USER_DIR)/dummy/%.S
	@echo + as[USER/tests] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
