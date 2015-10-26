	.set PROT_MODE_CSEG, 0x8	# kernel code segment selector
	.set PROT_MODE_DSEG, 0x10	# kernel data segment selector
	.set CR0_PE_ON, 0x1		# protected mode enable flag

	.globl start_ap, end_ap
start_ap:
	.code16
  cli
	cld

	xorw	%ax, %ax
	movw    %ax, %ds
	movw    %ax, %es
	movw    %ax, %ss

	lgdt	gdtdesc
	movl	%cr0, %eax
	orl	$CR0_PE_ON, %eax
	movl	%eax, %cr0

	ljmp	$PROT_MODE_CSEG, $protcseg

	.code32
protcseg:
	movw	$PROT_MODE_DSEG, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	xorw	%ax, %ax
	movw	%ax, %fs
	movw	%ax, %gs

	movl	start_ap - 4, %esp
	movl	start_ap - 8, %eax
	pushl	%eax
	call	*(start_ap - 12)

spin:
	hlt

	.p2align	2	/* force 4-byte alignment */
/* bootstrap GDT */
gdt:
	.word	0, 0
	.byte	0, 0, 0, 0

	/* code segment */
	.word	0xFFFF, 0
	.byte	0, 0x9A, 0xCF, 0

	/* data segment */
	.word	0xFFFF, 0
	.byte	0, 0x92, 0xCF, 0

/* GDT descriptor */
gdtdesc:
	.word	0x17			/* limit */
	.long	gdt			/* addr */
