/*
 * boot0
 *
 * - boot0 is put in the MBR and therefore should be smaller than 446 bytes.
 *
 * - boot0 is loaded to physical memory address 0x7c00 and the invoked by BIOS.
 *
 * - boot0 loads boot1 to 0x7e00. boot1 is put in the sectors after MBR and
 *   before the first partition, which are usually from sector 2 to sector 63.
 *
 * - boot0 requires
 *   1. boot0, boot1 and OS kernel are on the same disk;
 *   2. the first 63 sectors of the disk is out of any partition;
 *   3. kernel is on the beginning of the first bootable partition.
 *
 * - boot0 is running in the real mode and does not switch to the
 *   protected mode.
 */

	.set BOOT0, 0x7c00		# load address
	.set BOOT1, 0x7e00		# start address of boot1

	.set PR_TABLE, BOOT0+0x1be	# address of the partition table
	.set PR_BOOTABLE, 0x80		# bootable partition

	.globl start
start:
	.code16
	cli
	cld

	/* clear segment registers */
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss

	/* store the booting device id */
	xorl	%eax, %eax
	movb	%dl, %al
	movl	%eax, BOOT0 - 4

	/* set up the stack */
	movw  $(BOOT0 - 4), %bp
	movw  $(BOOT0 - 4), %sp

	/* set to normal (80x25 text) video mode */
set_video_mode:
	movw	$0x03, %ax
	int	$0x10

	/* print starting message */
	movw	$STARTUP_MSG, %si
	call	putstr

	/*
	 * load boot1 from sector 2 to sector 63
	 */
load_boot1:
	/* read disk */
	pushl	$0x0			# set the start
	pushl	$0x1			# LBA address
	pushw	%es			# set the buffer address
	pushw	$BOOT1			# to 0x7e00
	pushw	$62			# set the number of sectors to be read
	pushw	$0x10			# set the size of DAP
	movw	%sp, %si		# set the DAP address
	movw	$0x4200, %ax		# ah = 0x42, al = 0x00
	int	$0x13			# read sectors
	jc	load_fail		# encouter errors

	jmp	BOOT1			# jump to boot1

load_fail:
	movw	$LOAD_FAIL_MSG, %si
	call	putstr

spin:
	hlt

/*
 * print a string (@ %si) to the screen
 */
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
	.ascii	"Start boot0 ...\r\n\0"

LOAD_FAIL_MSG:
	.ascii	"Error during loading boot1.\r\n\0"


