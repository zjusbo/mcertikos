# -*-Makefile-*-

OBJDIRS		+= $(USER_OBJDIR)/pingpong

USER_PING_SRC	+= $(USER_DIR)/pingpong/ping.c
USER_PING_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_PING_SRC))
USER_PING_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_PING_OBJ))
KERN_BINFILES	+= $(USER_OBJDIR)/pingpong/ping

USER_PONG_SRC	+= $(USER_DIR)/pingpong/pong.c
USER_PONG_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_PONG_SRC))
USER_PONG_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_PONG_OBJ))
KERN_BINFILES	+= $(USER_OBJDIR)/pingpong/pong

USER_DING_SRC	+= $(USER_DIR)/pingpong/ding.c
USER_DING_OBJ	:= $(patsubst %.c, $(OBJDIR)/%.o, $(USER_DING_SRC))
USER_DING_OBJ	:= $(patsubst %.S, $(OBJDIR)/%.o, $(USER_DING_OBJ))
KERN_BINFILES	+= $(USER_OBJDIR)/pingpong/ding

pingpong: $(USER_OBJDIR)/pingpong/ping \
          $(USER_OBJDIR)/pingpong/pong \
          $(USER_OBJDIR)/pingpong/ding \

$(USER_OBJDIR)/pingpong/ping: $(USER_LIB_OBJ) $(USER_PING_OBJ)
	@echo + ld[USER/ping] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_PING_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/pingpong/pong: $(USER_LIB_OBJ) $(USER_PONG_OBJ)
	@echo + ld[USER/pong] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_PONG_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/pingpong/ding: $(USER_LIB_OBJ) $(USER_DING_OBJ)
	@echo + ld[USER/ding] $@
	$(V)$(LD) -o $@ $(USER_LDFLAGS) $(USER_LIB_OBJ) $(USER_DING_OBJ) $(GCC_LIBS)
	$(V)$(OBJDUMP) -S $@ > $@.asm
	$(V)$(NM) -n $@ > $@.sym

$(USER_OBJDIR)/pingpong/%.o: $(USER_DIR)/pingpong/%.c
	@echo + cc[USER/pingpong] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<

$(USER_OBJDIR)/pingpong/%.o: $(USER_DIR)/pingpong/%.S
	@echo + as[USER/pingpong] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(USER_CFLAGS) -c -o $@ $<
