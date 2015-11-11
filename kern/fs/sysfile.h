//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into inode.c, dir.c, path.c and file.c.
//

void sys_read(tf_t *tf);
void sys_write(tf_t *tf);
void sys_close(tf_t *tf);
void sys_fstat(tf_t *tf);
void sys_link(tf_t *tf);
void sys_unlink(tf_t *tf);
void sys_open(tf_t *tf);
void sys_mkdir(tf_t *tf);
void sys_chdir(tf_t *tf);
