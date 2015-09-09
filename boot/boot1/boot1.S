/*
 * boot1
 *
 * - boot1 is put just after MBR and before the first partition, i.e. from
 *   sector 2 to sector 63.
 *
 * - boot1 is loaded at 0x7e00 by boot0. boot0 passes one parameters to boot1,
 *   which is the booting device id.
 *
 * - boot1 loads kernel on the first bootable partition.
 *
 *   the memory layout at start is as described in following figure:
 *      :                              : (stack starts from here)
 *      +------------------------------+ <- BOOT0 - 4
 *      |   the booting device id      |
 *      +------------------------------+ <- BOOT0 (0x7c00)
 *      |             BOOT0            |
 *      |     (partition table)  0x55aa|
 *      +------------------------------+ <- BOOT1 (0x7e00)
 *      |                              |
 *      |                        [smap]|
 *      +------------------------------+
 *      :                              :
 *
 */

 	.set BOOT0, 0x7c00		# load address
	.set BOOT1, 0x7e00		# start address of boot1

	.set PR_TABLE, BOOT0+0x1be	# address of the partition table
	.set PR_BOOTABLE, 0x80		# bootable partition


	.set PROT_MODE_CSEG, 0x8	# kernel code segment selector
	.set PROT_MODE_DSEG, 0x10	# kernel data segment selector
	.set CR0_PE_ON, 0x1		# protected mode enable flag

	.set SMAP_SIG, 0x0534D4150	# "SMAP"

	.globl start
start:
	.code16
	cli
	cld

	/* enable A20 */
seta20.1:
	inb	$0x64, %al
	testb	$0x2, %al
	jnz	seta20.1
	movb	$0xd1, %al
	outb	%al, $0x64
seta20.2:
	inb	$0x64, %al
	testb	$0x2, %al
	jnz	seta20.2
	movb	$0xdf, %al
	outb	%al, $0x60

	/*
	 * print starting message
	 */
set_video_mode.2:
	movw	$STARTUP_MSG, %si
	call	putstr

	/*
	 * detect the physical memory map
	 */
e820:
	xorl	%ebx, %ebx		# ebx must be 0 when first calling e820
	movl	$SMAP_SIG, %edx		# edx must be 'SMAP' when calling e820
	movw	$(smap+4), %di		# set the address of the output buffer
e820.1:
	movl	$20, %ecx		# set the size of the output buffer
	movl	$0xe820, %eax		# set the BIOS service code
	int	$0x15			# call BIOS service e820h
e820.2:
	jc	e820.fail		# error during e820h
	cmpl	$SMAP_SIG, %eax		# check eax, which should be 'SMAP'
	jne	e820.fail
e820.3:
	movl	$20, -4(%di)
	addw	$24, %di
	cmpl	$0x0, %ebx		# whether it's the last descriptor
	je	e820.4
	jmp	e820.1
e820.4:					# zero the descriptor after the last one
	xorb	%al, %al
	movw	$20, %cx
	rep	stosb
	jmp	switch_prot
e820.fail:
	movw	$E820_FAIL_MSG, %si
	call	putstr
	jmp	spin16

spin16:
	hlt

	/*
	 * load the bootstrap GDT
	 */
switch_prot:
	lgdt	gdtdesc
	movl	%cr0, %eax
	orl	$CR0_PE_ON, %eax
	movl	%eax, %cr0
	/*
	 * switch to the protected mode
	 */
	ljmp	$PROT_MODE_CSEG, $protcseg

	.code32
protcseg:
	movw	$PROT_MODE_DSEG, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	/*
	 * jump to the C part
	 * (dev, lba, smap)
	 */
	pushl	$smap
	pushl	$BOOT0
	movl	(BOOT0-4), %eax
	pushl	%eax
	call	boot1main

spin:
	hlt

/*
 * print a string (@ %si) to the screen
 */
	.globl putstr
putstr:
	pusha
	movb	$0xe, %ah
putstr.1:
	lodsb
	cmp	$0, %al
	je	putstr.2
	int	$0x10
	jmp	putstr.1
putstr.2:
	popa
	ret

STARTUP_MSG:
	.ascii	"Start boot1 ...\r\n\0"

E820_FAIL_MSG:
	.ascii	"error when detecting memory map\r\n\0"

NO_BOOTABLE_MSG:
	.ascii	"No bootable partition.\r\n\0"

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

	/* 16 bit real mode CS */
	.word	0xFFFF, 0
	.byte	0, 0x9E, 0, 0

	/* 16 bit real mode DS */
	.word	0xFFFF, 0
	.byte	0, 0x92, 0, 0

/* GDT descriptor */
gdtdesc:
	.word	0x27			/* limit */
	.long	gdt			/* addr */

/* reserve space for memory map */
smap:
	.space 0xc00, 0
