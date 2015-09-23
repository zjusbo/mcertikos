# -*-Makefile-*-

OBJDIRS		+= $(OBJDIR)/user/lib

USER_LIB_SRC	+= $(USER_TOP)/lib/entry.S
USER_LIB_SRC	+= $(USER_TOP)/lib/debug.c
USER_LIB_SRC	+= $(USER_TOP)/lib/atoi.c
USER_LIB_SRC	+= $(USER_TOP)/lib/printf.c
USER_LIB_SRC	+= $(USER_TOP)/lib/printfmt.c
USER_LIB_SRC	+= $(USER_TOP)/lib/string.c
USER_LIB_SRC	+= $(USER_TOP)/lib/fake_syscall.c

USER_LIB_SRC	:= $(wildcard $(USER_LIB_SRC))
USER_LIB_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_LIB_SRC))
USER_LIB_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_LIB_OBJ))

user_lib: $(USER_LIB_OBJ)
	@echo All targets of lib are done.

$(OBJDIR)/user/lib/%.o: $(USER_TOP)/lib/%.c
	@echo + cc[USER/lib] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(OBJDIR)/user/lib/%.o: $(USER_TOP)/lib/%.S
	@echo + as[USER/lib] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
