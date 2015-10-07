# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/lib

USER_LIB_SRC	+= $(USER_DIR)/lib/entry.S
USER_LIB_SRC	+= $(USER_DIR)/lib/debug.c
USER_LIB_SRC	+= $(USER_DIR)/lib/atoi.c
USER_LIB_SRC	+= $(USER_DIR)/lib/printf.c
USER_LIB_SRC	+= $(USER_DIR)/lib/printfmt.c
USER_LIB_SRC	+= $(USER_DIR)/lib/proc.c
USER_LIB_SRC	+= $(USER_DIR)/lib/spinlock.c
USER_LIB_SRC	+= $(USER_DIR)/lib/string.c

USER_LIB_SRC	:= $(wildcard $(USER_LIB_SRC))
USER_LIB_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_LIB_SRC))
USER_LIB_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_LIB_OBJ))

lib: $(USER_LIB_OBJ)
	@echo All targets of lib are done.

$(USER_OBJDIR)/lib/%.o: $(USER_DIR)/lib/%.c
	@echo + cc[USER/lib] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/lib/%.o: $(USER_DIR)/lib/%.S
	@echo + as[USER/lib] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
