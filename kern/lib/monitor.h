#ifndef _KERN_LIB_MONITOR_H
#define _KERN_LIB_MONITOR_H

#ifdef _KERN_

struct Trapframe;

// Activate the kernel monitor,
// optionally providing a trap frame indicating the current state
// (NULL if none).
void monitor(struct Trapframe *tf);

// Functions implementing monitor commands.
int mon_start_user(int argc, char **argv, struct Trapframe *tf);
int mon_help(int argc, char **argv, struct Trapframe *tf);
int mon_kerninfo(int argc, char **argv, struct Trapframe *tf);
int mon_backtrace(int argc, char **argv, struct Trapframe *tf);

#endif /* _KERN_ */

#endif	// !_KERN_LIB_MONITOR_H
