/*
 * void cswitch(struct kctx *from, struct kctx *to);
 */

	.globl cswitch
cswitch:
	movl	  4(%esp), %eax	/* %eax <- from */
	movl	  8(%esp), %edx	/* %edx <- to */

	/* TODO: save the old kernel context */

	/* TODO: load the new kernel context */

	xor     %eax, %eax
	ret
