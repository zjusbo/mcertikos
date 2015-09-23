	.text

	.globl _start
_start:
	pushal

	call init

	/* Jump to the C part. */

	call	main
	popal
	ret
