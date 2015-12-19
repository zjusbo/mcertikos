#include <lib/debug.h>
#include <lib/types.h>
#include <lib/x86.h>
#include <lib/trap.h>
#include <lib/syscall.h>
#include <dev/intr.h>
#include <lib/ipc.h>
#include <pcpu/PCPUIntro/export.h>
#include <thread/PTCBIntro/export.h>
#include <fs/dir.h>
#include <fs/inode.h>
#include "import.h"

extern struct MsgBlock msgBlock[NUM_IDS];
extern spinlock_t msg_lock;

// recv_id, user_buffer_addr, length
void sys_sync_send(tf_t *tf){
   unsigned int cur_pid;
   unsigned int recv_pid, user_addr, length;
//   spinlock_acquire(&msg_lock); 
   recv_pid = syscall_get_arg2(tf);
   user_addr = syscall_get_arg3(tf);
   length = syscall_get_arg4(tf);
   cur_pid = get_curid();
   msgBlock[cur_pid].recv_pid = recv_pid; 
   msgBlock[cur_pid].buffer_addr = user_addr;
   msgBlock[cur_pid].length = length;
   msg_enqueue(cur_pid);
   thread_wakeup(&msgBlock[cur_pid].send_cv); 
   // cur_pid has not been read, block and wait for it
   while(msg_getBlockBySendID(cur_pid) != NUM_IDS){
      thread_sleep(&msgBlock[cur_pid].recv_cv, &msg_lock);
   } 
   syscall_set_errno(tf, E_SUCC);
//   spinlock_release(&msg_lock);  
   return ;
}

void sys_sync_recv(tf_t *tf){
   unsigned int cur_pid;
   unsigned int send_pid, user_recv_addr, recv_length, send_length, copy_length, user_send_addr;
//   spinlock_acquire(&msg_lock); 
   
   send_pid = syscall_get_arg2(tf);
   user_recv_addr = syscall_get_arg3(tf);
   recv_length = syscall_get_arg4(tf);
   cur_pid = get_curid();
   
   // loop if current pid has no received message or received message is not sent from target process
   while(msg_getBlockBySendID(send_pid) == NUM_IDS || msgBlock[send_pid].recv_pid != cur_pid){
      thread_sleep(&msgBlock[send_pid].send_cv, &msg_lock);
   }
   // message received
   user_send_addr = msgBlock[send_pid].buffer_addr;
   send_length = msgBlock[send_pid].length;

   // find the min of send_length and recv_length, then copy data from source process addressing space to dest process addressing space 
   copy_length = send_length < recv_length? send_length: recv_length;
   ipc_copy(cur_pid, user_recv_addr, send_pid, user_send_addr, copy_length);
   msg_remove(send_pid);
   thread_wakeup(&msgBlock[send_pid].recv_cv);
   syscall_set_errno(tf, E_SUCC);
   syscall_set_retval1(tf, copy_length);
//   spinlock_release(&msg_lock);
   return ; 
}

static char sys_buf[NUM_IDS][PAGESIZE];

/**
 * Copies a string from user into buffer and prints it to the screen.
 * This is called by the user level "printf" library as a system call.
 */
void sys_puts(tf_t *tf)
{
  unsigned int cur_pid;
  unsigned int str_uva, str_len;
  unsigned int remain, cur_pos, nbytes;

  cur_pid = get_curid();
  str_uva = syscall_get_arg2(tf);
  str_len = syscall_get_arg3(tf);

  if (!(VM_USERLO <= str_uva && str_uva + str_len <= VM_USERHI)) {
    syscall_set_errno(tf, E_INVAL_ADDR);
    return;
  }

  remain = str_len;
  cur_pos = str_uva;

  while (remain) {
    if (remain < PAGESIZE - 1)
      nbytes = remain;
    else
      nbytes = PAGESIZE - 1;

    if (pt_copyin(cur_pid,
		  cur_pos, sys_buf[cur_pid], nbytes) != nbytes) {
      syscall_set_errno(tf, E_MEM);
      return;
    }

    sys_buf[cur_pid][nbytes] = '\0';
    KERN_INFO("%s", sys_buf[cur_pid]);

    remain -= nbytes;
    cur_pos += nbytes;
  }

  syscall_set_errno(tf, E_SUCC);
}

extern uint8_t _binary___obj_user_pingpong_ping_start[];
extern uint8_t _binary___obj_user_pingpong_pong_start[];
extern uint8_t _binary___obj_user_pingpong_ding_start[];
extern uint8_t _binary___obj_user_fstest_fstest_start[];
extern uint8_t _binary___obj_user_shell_shell_start[];
/**
 * Spawns a new child process.
 * The user level library function sys_spawn (defined in user/include/syscall.h)
 * takes two arguments [elf_id] and [quota], and returns the new child process id
 * or NUM_IDS (as failure), with appropriate error number.
 * Currently, we have three user processes defined in user/pingpong/ directory,
 * ping, pong, and ding.
 * The linker ELF addresses for those compiled binaries are defined above.
 * Since we do not yet have a file system implemented in mCertiKOS,
 * we statically loading the ELF binraries in to the memory based on the
 * first parameter [elf_id], i.e., ping, pong, and ding corresponds to
 * the elf_id of 1, 2, and 3, respectively.
 * If the parameter [elf_id] is none of those three, then it should return
 * NUM_IDS with the error number E_INVAL_PID. The same error case apply
 * when the proc_create fails.
 * Otherwise, you mark it as successful, and return the new child process id.
 */
void sys_spawn(tf_t *tf)
{
  unsigned int new_pid;
  unsigned int elf_id, quota;
  void *elf_addr;
  unsigned int qok, nc, curid;

  elf_id = syscall_get_arg2(tf);
  quota = syscall_get_arg3(tf);

  qok = container_can_consume(curid, quota);
  nc = container_get_nchildren(curid);
  curid = get_curid();
  if (qok == 0) {
    syscall_set_errno(tf, E_EXCEEDS_QUOTA);
    syscall_set_retval1(tf, NUM_IDS);
    return;
  }
  else if (NUM_IDS < curid * MAX_CHILDREN + 1 + MAX_CHILDREN) {
    syscall_set_errno(tf, E_MAX_NUM_CHILDEN_REACHED);
    syscall_set_retval1(tf, NUM_IDS);
    return;
  }
  else if (nc == MAX_CHILDREN) {
    syscall_set_errno(tf, E_INVAL_CHILD_ID);
    syscall_set_retval1(tf, NUM_IDS);
    return;
  }

  if (elf_id == 1) {
    elf_addr = _binary___obj_user_pingpong_ping_start;
  } else if (elf_id == 2) {
    elf_addr = _binary___obj_user_pingpong_pong_start;
  } else if (elf_id == 3) {
    elf_addr = _binary___obj_user_pingpong_ding_start;
  } else if (elf_id == 4) {
    elf_addr = _binary___obj_user_fstest_fstest_start;
  } else if (elf_id == 5) {
    elf_addr = _binary___obj_user_shell_shell_start;
  } else {
    syscall_set_errno(tf, E_INVAL_PID);
    syscall_set_retval1(tf, NUM_IDS);
    return;
  }

  new_pid = proc_create(elf_addr, quota);

  if (new_pid == NUM_IDS) {
    syscall_set_errno(tf, E_INVAL_PID);
    syscall_set_retval1(tf, NUM_IDS);
  } else {
    syscall_set_errno(tf, E_SUCC);
    syscall_set_retval1(tf, new_pid);
  }
}

/**
 * Yields to another thread/process.
 * The user level library function sys_yield (defined in user/include/syscall.h)
 * does not take any argument and does not have any return values.
 * Do not forget to set the error number as E_SUCC.
 */
void sys_yield(tf_t *tf)
{
  thread_yield();
  syscall_set_errno(tf, E_SUCC);
}
void sys_is_dir(tf_t * tf){
  int fd, type, isDir;
  struct file * fp;
  fd = syscall_get_arg2(tf);
  fp = tcb_get_openfiles(get_curid())[fd];
  if(fp == 0 || fp->ip == 0){
    KERN_INFO("kern/trap/TSyscall/TSyscall:sys_is_dir: fp illegal\n");
    syscall_set_retval1(tf, -1);
    syscall_set_errno(tf, E_BADF);
    return ;
  } 
  type = fp->ip->type;
  isDir = (type == T_DIR);
  syscall_set_errno(tf, E_SUCC);
  syscall_set_retval1(tf, isDir);
}
void sys_produce(tf_t *tf)
{
  unsigned int i;
  for(i = 0; i < 5; i++) {
    intr_local_disable();
    KERN_DEBUG("CPU %d: Process %d: Produced %d\n", get_pcpu_idx(), get_curid(), i);
    intr_local_enable();
  }
  syscall_set_errno(tf, E_SUCC);
}

void sys_consume(tf_t *tf)
{
  unsigned int i;
  for(i = 0; i < 5; i++) {
    intr_local_disable();
    KERN_DEBUG("CPU %d: Process %d: Consumed %d\n", get_pcpu_idx(), get_curid(), i);
    intr_local_enable();
  }
  syscall_set_errno(tf, E_SUCC);
}
void sys_ls(tf_t *tf)
{
  uint32_t off, inum;
  struct dirent de;
  uint32_t de_size;
  struct inode * p_inode;
  int user_buf_addr = syscall_get_arg2(tf);
  int buf_len = syscall_get_arg3(tf);
  int cur_pid = get_curid();
  int len;
  char * buf_p = sys_buf[cur_pid];
  struct inode* dp = (struct inode*)tcb_get_cwd(cur_pid);  
  de_size = sizeof(de);
  for(off = 0; off < dp->size; off += de_size){
    if(inode_read(dp, (char *)&de, off, de_size)!= de_size){
        // size is not legal
        KERN_PANIC("wrong in dir_lookup"); 
    }
    if(de.inum == 0){
        // free entry
        continue;
    }
    // TODO index may out of bound  
    strncpy(buf_p, de.name, strnlen(de.name, PAGESIZE));
    buf_p += strnlen(de.name, PAGESIZE);
    *(buf_p++) = ' ';  
    //dprintf("%s ", de.name);
  }
  *(buf_p - 1) = '\0';
  //dprintf("\n");
  len = buf_p - sys_buf[cur_pid] < buf_len? buf_p - sys_buf[cur_pid]: buf_len;
  pt_copyout(sys_buf[cur_pid], cur_pid, user_buf_addr, len);
  syscall_set_errno(tf, E_SUCC);
  syscall_set_retval1(tf, len);
}
void sys_pwd(tf_t *tf)
{
  uint32_t poff;
  struct inode* curi = (struct inode*)tcb_get_cwd(get_curid());
  struct inode* parent = dir_lookup(curi, "..", &poff);
  char arr[10][16];
  int len = 0;
  struct dirent de;
  uint32_t de_size = sizeof(de);
  uint32_t off;
  int user_buf;
  user_buf = syscall_get_arg2(tf);
  char *p = sys_buf[get_curid()];
  while (parent->inum != curi->inum) {
    for (off = 0; off < parent->size; off += de_size) {
      if (inode_read(parent, (char*)&de, off, de_size) != de_size) {
         KERN_PANIC("wrong in dir_lookup");
      }
      if (de.inum == curi->inum) {
        strncpy(arr[len], de.name, 15);
        len++;
        break;
      }
    }
    curi = parent;
    parent = dir_lookup(curi, "..", &poff);
  }
  strncpy(arr[len], "/", 5);
  while (len >= 0) {
    strncpy(p, arr[len], PAGESIZE);
    p += strnlen(arr[len]);
    *(p++) = '/';
    len--;
    //dprintf("%s/", arr[len--]);
  }
  *(--p) = '\0';
  pt_copyout(sys_buf[get_curid()], get_curid(), user_buf, p - sys_buf[get_curid()] + 1);  
  syscall_set_errno(tf, E_SUCC);
  syscall_set_retval1(tf, 0);
}


void sys_mv(tf_t *tf)
{
}

void sys_cat(tf_t *tf)
{
}

void sys_rm(tf_t *tf)
{
  int isRecursive;
  int user_addr_path;
  int length; 
  int cur_pid;
  user_addr_path = syscall_get_arg2(tf);
  length = syscall_get_arg3(tf);
  isRecursive = syscall_get_arg4(tf);
  cur_pid = get_curid();
  length = length < PAGESIZE - 1? length: PAGESIZE - 1;
  pt_copyin(cur_pid, user_addr_path, sys_buf[cur_pid], length);
  sys_buf[cur_pid][length] = '\0'; // path
  // TODO unfinished 
 
}

void sys_cp(tf_t *tf)
{
}

void sys_readline(tf_t *tf)
{
  char* kernbuf = (char*)readline(">:");
  char* userbuf = (char*)syscall_get_arg2(tf);
  int n_len = strnlen(kernbuf, 1000) + 1;

  if (pt_copyout((void*)kernbuf, get_curid(), userbuf, n_len) != n_len) {
    KERN_PANIC("Readline fails!\n");
    syscall_set_errno(tf, E_MEM);
    syscall_set_retval1(tf, -1);
  }
  syscall_set_errno(tf, E_SUCC);
  syscall_set_retval1(tf, 0);
}
void sys_touch(tf_t *tf)
{
}
