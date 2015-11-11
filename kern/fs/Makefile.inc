# -*-Makefile-*-

OBJDIRS		+= $(KERN_OBJDIR)/fs

KERN_SRCFILES	+= $(KERN_DIR)/fs/bufcache.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/log.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/block.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/inode.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/dir.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/path.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/file.c
KERN_SRCFILES	+= $(KERN_DIR)/fs/sysfile.c

$(KERN_OBJDIR)/fs/%.o: $(KERN_DIR)/fs/%.c
	@echo + cc[KERN/fs] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<

$(KERN_OBJDIR)/fs/%.o: $(KERN_DIR)/fs/%.S
	@echo + as[KERN/fs] $<
	@mkdir -p $(@D)
	$(V)$(CC) $(KERN_CFLAGS) -c -o $@ $<
