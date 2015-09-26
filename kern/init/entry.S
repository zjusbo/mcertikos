/* See COPYRIGHT for copyright information. */

#define MULTIBOOT_PAGE_ALIGN  (1<<0)
#define MULTIBOOT_MEMORY_INFO (1<<1)
#define MULTIBOOT_HEADER_MAGIC (0x1BADB002)
#define MULTIBOOT_BOOTLOADER_MAGIC (0x2BADB002)
#define MULTIBOOT_HEADER_FLAGS (MULTIBOOT_MEMORY_INFO | MULTIBOOT_PAGE_ALIGN)
#define CHECKSUM (-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS))

	.text

	/* The Multiboot header */
	.align	4
	.long	MULTIBOOT_HEADER_MAGIC
	.long	MULTIBOOT_HEADER_FLAGS
	.long	CHECKSUM

	/* this is the entry of the kernel */
	.globl	start
start:
	cli

	/* check whether the bootloader provide multiboot information */
	cmpl    $MULTIBOOT_BOOTLOADER_MAGIC, %eax
	jne     spin
	movl	%ebx, multiboot_ptr

	/* tell BIOS to warmboot next time */
	movw	$0x1234,0x472

	/* clear EFLAGS */
	pushl	$0x2
	popfl

	/* prepare the kernel stack  */
	movl	$0x0,%ebp
	movl	$(bsp_kstack+4096),%esp

	/* jump to the C code */
	push	multiboot_ptr
	call	kern_init

	/* should not be here */
spin:
	hlt

multiboot_ptr:
	.long	0x00000000
	.align	4
