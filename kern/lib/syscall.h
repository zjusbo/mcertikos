#ifndef _KERN_LIB_SYSCALL_H_
#define _KERN_LIB_SYSCALL_H_

/*
 * Calling conventions of system calls in CertiKOS:
 *
 * 1. All system calls are triggered by the soft interrupt #T_SYSCALL in ring 3.
 *
 * 2. The caller in ring 3 should identify the system call number and the
 *    necessary arguments before triggering the soft interrrupt #48.
 *
 * 3. The system call number is identified via the value in EAX. All valid
 *    system call numbers are listed in __syscall_nr (except MAX_SYSCALL_NR).
 *    Any system call with an invalid system call number will return with
 *    an error code E_INVAL_CALLNR.
 *
 * 4. A system call can take at most 5 arguments which are passed via registers
 *    EBX, ECX, EDX, ESI and EDI.
 *
 * 5. A system call always returns with an error number via register EAX.  All
 *    valid error numbers are listed in __error_nr. E_SUCC indicates no errors
 *    happen.
 *
 * 6. A system call can return at most 5 32-bit values via registers EBX, ECX,
 *    EDX, ESI and EDI.
 */

#define T_SYSCALL	48

enum __syscall_nr {
	/*
	 * common system calls
	 */
	SYS_puts = 0,	/* output a string to the screen */
	SYS_spawn,	/* create a new process */

	SYS_yield,	/* yield to another process */

	MAX_SYSCALL_NR	/* XXX: always put it at the end of __syscall_nr */
};

enum __error_nr {
	E_SUCC,		/* no errors */
	E_MEM,		/* memory failure */
	E_IPC,
	E_INVAL_CALLNR,	/* invalid syscall number */
	E_INVAL_ADDR,	/* invalid address */
	E_INVAL_PID,	/* invalid process ID */
	E_INVAL_REG,
	E_INVAL_SEG,
	E_INVAL_EVENT,
	E_INVAL_PORT,
	E_INVAL_HVM,
	E_INVAL_CHID,
	E_INVAL_ID,     /* general invalid id */
	E_DISK_OP,	/* disk operation failure */
	E_HVM_VMRUN,
	E_HVM_MMAP,
	E_HVM_REG,
	E_HVM_SEG,
	E_HVM_NEIP,
	E_HVM_INJECT,
	E_HVM_IOPORT,
	E_HVM_MSR,
	E_HVM_INTRWIN,
	MAX_ERROR_NR	/* XXX: always put it at the end of __error_nr */
};

#define DISK_READ	0
#define DISK_WRITE	1

typedef enum {
	GUEST_EAX, GUEST_EBX, GUEST_ECX, GUEST_EDX, GUEST_ESI, GUEST_EDI,
	GUEST_EBP, GUEST_ESP, GUEST_EIP, GUEST_EFLAGS,
	GUEST_CR0, GUEST_CR2, GUEST_CR3, GUEST_CR4,
	GUEST_MAX_REG
} guest_reg_t;

typedef enum {
	GUEST_CS, GUEST_DS, GUEST_ES, GUEST_FS, GUEST_GS, GUEST_SS,
	GUEST_LDTR, GUEST_TR, GUEST_GDTR, GUEST_IDTR,
	GUEST_MAX_SEG_DESC
} guest_seg_t;

#endif /* !_KERN_LIB_SYSCALL_H_ */
