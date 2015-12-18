//
// File-system system calls.
//

#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/string.h>
#include <kern/thread/PTCBIntro/export.h>
#include <kern/lib/trap.h>
#include <kern/lib/syscall.h>
#include <kern/trap/TSyscallArg/export.h>
#include <kern/lib/spinlock.h>

#include "dir.h"
#include "path.h"
#include "file.h"
#include "fcntl.h"

/**
 * This function is not a system call handler, but an auxiliary function
 * used by sys_open.
 * Allocate a file descriptor for the given file.
 * You should scan the list of open files for the current thread
 * and find the first file descriptor that is available.
 * Return the found descriptor or -1 if none of them are free.
 */
static int
fdalloc(struct file *f)
{
  //TODO
  int idx;
  int curid = get_curid();
  struct file ** fds = tcb_get_openfiles(curid);
  for(idx = 0; idx < NOFILE; idx++){ 
    if(fds[idx] == 0){
      tcb_set_openfiles(curid, idx, f);
      return idx;
    }
  }
  return -1;
}


/**
 * From the file indexed by the given file descriptor, read n bytes and save them
 * into the buffer in the user. As explained in the assignment specification,
 * you should first write to a kernel buffer then copy the data into user buffer
 * with pt_copyout.
 * Return Value: Upon successful completion, read() shall return a non-negative
 * integer indicating the number of bytes actually read. Otherwise, the
 * functions shall return -1 and set errno E_BADF to indicate the error.
 */
char kern_buf[10000];
extern spinlock_t buf_lock;
void sys_read(tf_t *tf)
{
  //TODO
  int fd;
  unsigned int user_buf;
  unsigned int  n;
  unsigned int pid = get_curid();
  struct file * fp;
  int size_read;
  spinlock_acquire(&buf_lock);
  fd = (int)syscall_get_arg2(tf);
  user_buf = syscall_get_arg3(tf);
  n = syscall_get_arg4(tf);
  if(fd < 0 || fd >= NOFILE || n < 0){
    KERN_INFO("fd illegal\n");
    syscall_set_errno(tf, E_BADF); // BAD FILE DESCRIPTRR
    syscall_set_retval1(tf, -1);
    spinlock_release(&buf_lock);
    return ;
  }
  if (user_buf < VM_USERLO || user_buf + n > VM_USERHI || n > sizeof(kern_buf)){
    KERN_INFO("user_buf illegal\n");
    KERN_INFO("user_buf = %u, n = %u, VM_USERLO = %u, VM_USERHI = %u\n", user_buf, n, VM_USERLO, VM_USERHI);
    syscall_set_errno(tf, E_INVAL_ADDR);
    syscall_set_retval1(tf, -1);
    spinlock_release(&buf_lock);
    return ;
  }
  fp = tcb_get_openfiles(pid)[fd];
  if(fp == 0 || fp->ip == 0){
    KERN_INFO("fp illegal\n");
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    spinlock_release(&buf_lock);
    return ;
  } 
  size_read = file_read(fp, kern_buf, n);
  if(size_read > 0)
       pt_copyout(kern_buf, get_curid(), user_buf, size_read); 
  syscall_set_retval1(tf, size_read);
  syscall_set_errno(tf, E_SUCC);
  spinlock_release(&buf_lock);
  return ;
}

/**
 * Write n bytes of data in the user's buffer into the file indexed by the file descriptor.
 * You should first copy the data info an in-kernel buffer with pt_copyin and then
 * pass this buffer to appropriate file manipulation function.
 * Upon successful completion, write() shall return the number of bytes actually
 * written to the file associated with f. This number shall never be greater
 * than nbyte. Otherwise, -1 shall be returned and errno E_BADF set to indicate the
 * error.
 */
void sys_write(tf_t *tf)
{
 //TODO
  int fd;
  unsigned int user_buf;
  unsigned n;
  unsigned int pid = get_curid();
  struct file * fp;
  int size_write;
  spinlock_acquire(&buf_lock);
  fd = (int)syscall_get_arg2(tf);
  user_buf = syscall_get_arg3(tf);
  n = syscall_get_arg4(tf);
  if(fd < 0 || fd >= NOFILE || n < 0){
    syscall_set_errno(tf, E_BADF); // BAD FILE DESCRIPTRR
    syscall_set_retval1(tf, -1);
    spinlock_release(&buf_lock);
    return ;
  }
  if (user_buf < VM_USERLO || user_buf + n > VM_USERHI || n > sizeof(kern_buf)){
    syscall_set_errno(tf, E_INVAL_ADDR);
    syscall_set_retval1(tf, -1);
    spinlock_release(&buf_lock);
    return ;
  }
  fp = tcb_get_openfiles(pid)[fd];
  if(fp == 0 || fp->ip == 0){
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    spinlock_release(&buf_lock);
    return ;
  } 
  pt_copyin(pid, user_buf, kern_buf, n); 
  size_write = file_write(fp, kern_buf, n);
  syscall_set_retval1(tf, size_write);
  syscall_set_errno(tf, E_SUCC);
  spinlock_release(&buf_lock);
  return ;
}


/**
 * Return Value: Upon successful completion, 0 shall be returned; otherwise, -1
 * shall be returned and errno E_BADF set to indicate the error.
 */
void sys_close(tf_t *tf)
{
  //TODO
  unsigned int fd;
  struct file * fp;
  unsigned int pid = get_curid();
  fd = syscall_get_arg2(tf);
  if(fd < 0 || fd >= NOFILE){
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    return ;
  }
  fp = tcb_get_openfiles(pid)[fd];
  if(fp == 0){ // fd does not exist
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    return ;
  }
  file_close(fp);
  tcb_set_openfiles(pid, fd, 0);
  syscall_set_retval1(tf, 0);
  syscall_set_errno(tf, E_SUCC);
  return ; 
}

/**
 * Return Value: Upon successful completion, 0 shall be returned. Otherwise, -1
 * shall be returned and errno E_BADF set to indicate the error.
 */
void sys_fstat(tf_t *tf)
{
  //TODO
  int fd = (int)syscall_get_arg2(tf);
  struct file_stat* user_stp = (struct file_stat *)syscall_get_arg3(tf);
  struct file_stat kern_st;
  struct file * fp;
  if(fd < 0 || fd >= NOFILE || (unsigned int)user_stp < VM_USERLO || (unsigned int)user_stp + sizeof(struct file_stat) > VM_USERHI){
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    return ;  
  }
  fp = tcb_get_openfiles(get_curid())[fd];
  if(fp == 0){ // fd does not exist
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    return ;
  }
 
  pt_copyin(get_curid(), user_stp, &kern_st, sizeof(struct file_stat));  
  int state = file_stat(fp, &kern_st); 
  syscall_set_retval1(tf, state);
  if(state == 0){
    syscall_set_errno(tf, E_SUCC);
  }
  else{
    syscall_set_errno(tf, E_BADF);
  }
  return ;
}

/**
 * Create the path new as a link to the same inode as old.
 */
void sys_link(tf_t *tf)
{
  char name[DIRSIZ], new[128], old[128];
  struct inode *dp, *ip;
  unsigned int old_len, new_len;
  old_len = syscall_get_arg4(tf);
  new_len = syscall_get_arg5(tf);
  old_len = old_len < 128? old_len: 127;
  new_len = new_len < 128? new_len: 127;
  pt_copyin(get_curid(), syscall_get_arg2(tf), old, old_len);
  pt_copyin(get_curid(), syscall_get_arg3(tf), new, new_len);
  old[old_len] = '\0';
  new[new_len] = '\0';
  if((ip = namei(old)) == 0){
    syscall_set_errno(tf, E_NEXIST);
    return;
  }
  
  begin_trans();

  inode_lock(ip);
  if(ip->type == T_DIR){
    inode_unlockput(ip);
    commit_trans();
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }

  ip->nlink++;
  inode_update(ip);
  inode_unlock(ip);

  if((dp = nameiparent(new, name)) == 0)
    goto bad;
  inode_lock(dp);
  if(   dp->dev != ip->dev
     || dir_link(dp, name, ip->inum) < 0){
    inode_unlockput(dp);
    goto bad;
  }
  inode_unlockput(dp);
  inode_put(ip);

  commit_trans();

  syscall_set_errno(tf, E_SUCC);
  return;

bad:
  inode_lock(ip);
  ip->nlink--;
  inode_update(ip);
  inode_unlockput(ip);
  commit_trans();
  syscall_set_errno(tf, E_DISK_OP);
  return;
}

/**
 * Is the directory dp empty except for "." and ".." ?
 */
static int
isdirempty(struct inode *dp)
{
  int off;
  struct dirent de;

  for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
    if(inode_read(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
      KERN_PANIC("isdirempty: readi");
    if(de.inum != 0)
      return 0;
  }
  return 1;
}

void sys_unlink(tf_t *tf)
{
  struct inode *ip, *dp;
  struct dirent de;
  char name[DIRSIZ], path[128];
  uint32_t off;
  unsigned int path_len = syscall_get_arg3(tf);
  path_len = path_len < 128? path_len: 127;
  pt_copyin(get_curid(), syscall_get_arg2(tf), path, path_len);
  path[path_len] = '\0';

  if((dp = nameiparent(path, name)) == 0){
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }
  
  begin_trans();

  inode_lock(dp);

  // Cannot unlink "." or "..".
  if(   dir_namecmp(name, ".") == 0
     || dir_namecmp(name, "..") == 0)
    goto bad;

  if((ip = dir_lookup(dp, name, &off)) == 0)
    goto bad;
  inode_lock(ip);

  if(ip->nlink < 1)
    KERN_PANIC("unlink: nlink < 1");
  if(ip->type == T_DIR && !isdirempty(ip)){
    inode_unlockput(ip);
    goto bad;
  }

  memset(&de, 0, sizeof(de));
  if(inode_write(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
    KERN_PANIC("unlink: writei");
  if(ip->type == T_DIR){
    dp->nlink--;
    inode_update(dp);
  }
  inode_unlockput(dp);

  ip->nlink--;
  inode_update(ip);
  inode_unlockput(ip);

  commit_trans();

  syscall_set_errno(tf, E_SUCC);
  return;
  
bad:
  inode_unlockput(dp);
  commit_trans();
  syscall_set_errno(tf, E_DISK_OP);
  return;
}

static struct inode*
create(char *path, short type, short major, short minor)
{
  uint32_t off;
  struct inode *ip, *dp;
  char name[DIRSIZ];

  if((dp = nameiparent(path, name)) == 0)
    return 0;
  inode_lock(dp);

  if((ip = dir_lookup(dp, name, &off)) != 0){
    inode_unlockput(dp);
    inode_lock(ip);
    if(type == T_FILE && ip->type == T_FILE)
      return ip;
    inode_unlockput(ip);
    return 0;
  }

  if((ip = inode_alloc(dp->dev, type)) == 0)
    KERN_PANIC("create: ialloc");

  inode_lock(ip);
  ip->major = major;
  ip->minor = minor;
  ip->nlink = 1;
  inode_update(ip);

  if(type == T_DIR){  // Create . and .. entries.
    dp->nlink++;  // for ".."
    inode_update(dp);
    // No ip->nlink++ for ".": avoid cyclic ref count.
    if(   dir_link(ip, ".", ip->inum) < 0
       || dir_link(ip, "..", dp->inum) < 0)
      KERN_PANIC("create dots");
  }

  if(dir_link(dp, name, ip->inum) < 0)
    KERN_PANIC("create: dir_link");

  inode_unlockput(dp);

  return ip;
}

void sys_open(tf_t *tf)
{
  char path[128];
  int fd, omode;
  struct file *f;
  struct inode *ip;
  unsigned int path_len;
  static int first = TRUE;
  if (first) {
    first = FALSE;
    log_init();
  }
  path_len = syscall_get_arg4(tf);
  path_len = path_len < 128? path_len: 127;
  pt_copyin(get_curid(), syscall_get_arg2(tf), path, path_len);

  path[path_len] = '\0';
  omode = syscall_get_arg3(tf);

  if (!path)
    KERN_PANIC("sys_open: no path");
  
  if(omode & O_CREATE){
    begin_trans();
    ip = create(path, T_FILE, 0, 0);
    commit_trans();
    if(ip == 0){
      syscall_set_retval1(tf, -1);
      syscall_set_errno(tf, E_CREATE);
      return;
    }
  } else {
    if((ip = namei(path)) == 0){
      syscall_set_retval1(tf, -1);
      syscall_set_errno(tf, E_NEXIST);
      return;
    }
    inode_lock(ip);
    if(ip->type == T_DIR && omode != O_RDONLY){
      inode_unlockput(ip);
      syscall_set_retval1(tf, -1);
      syscall_set_errno(tf, E_DISK_OP);
      return;
    }
  }

  if((f = file_alloc()) == 0 || (fd = fdalloc(f)) < 0){
    if(f)
      file_close(f);
    inode_unlockput(ip);
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }
  inode_unlock(ip);

  f->type = FD_INODE;
  f->ip = ip;
  f->off = 0;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
  syscall_set_retval1(tf, fd);
  syscall_set_errno(tf, E_SUCC);
}

void sys_mkdir(tf_t *tf)
{
  char path[128];
  struct inode *ip;
  unsigned int path_len = syscall_get_arg3(tf);
  path_len = path_len < 128? path_len: 127;
  pt_copyin(get_curid(), syscall_get_arg2(tf), path, path_len);
  path[path_len] = '\0';

  begin_trans();
  if((ip = (struct inode*)create(path, T_DIR, 0, 0)) == 0){
    commit_trans();
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }
  inode_unlockput(ip);
  commit_trans();
  syscall_set_errno(tf, E_SUCC);
}

void sys_chdir(tf_t *tf)
{
  char path[128];
  struct inode *ip;
  int pid = get_curid();
  unsigned int path_len = syscall_get_arg3(tf); 
  path_len = path_len < 128? path_len: 127;
  pt_copyin(get_curid(), syscall_get_arg2(tf), path, path_len);
  path[path_len] = '\0';

  if((ip = namei(path)) == 0){
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }
  inode_lock(ip);
  if(ip->type != T_DIR){
    inode_unlockput(ip);
    syscall_set_errno(tf, E_DISK_OP);
    return;
  }
  inode_unlock(ip);
  inode_put(tcb_get_cwd(pid));
  tcb_set_cwd(pid, ip);
  syscall_set_errno(tf, E_SUCC);
}

