	.set  MBOOT_INFO_MAGIC, 0x2badb002

	.globl	exec_kernel
	.code32
exec_kernel:
	cli
	movl	$MBOOT_INFO_MAGIC, %eax
	movl	8(%esp), %ebx
	movl	4(%esp), %edx
	jmp	*%edx
