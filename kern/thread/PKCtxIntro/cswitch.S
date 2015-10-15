/*
 * void cswitch(struct kctx *from, struct kctx *to);
 */

	.globl cswitch
cswitch:
	movl	  4(%esp), %eax	/* %eax <- from */
	movl	  8(%esp), %edx	/* %edx <- to */

	/* TODO: save the old kernel context */
//SBO CODE START
        movl	  %esp, 0(%eax) //%eax is the bass address of struct kctx of current process
        movl 	  %edi, 4(%eax)
        movl      %esi, 8(%eax)
        movl      %ebx, 12(%eax)
        movl      %ebp, 16(%eax)
        movl      0(%esp), %ecx    //return address of current function
        movl      %ecx, 20(%eax)  //return address of current function, when current process is resumed again, it will start at the line 'ret'.
	/* TODO: load the new kernel context */
     	movl      0(%edx), %esp
     	movl      4(%edx), %edi
     	movl      8(%edx), %esi
     	movl      12(%edx), %ebx
     	movl      16(%edx), %ebp
        movl      20(%edx), %ecx
     	movl      %ecx, 0(%esp) //change the return address of current process. Actually, after all the five registers are loaded from the new process. When program executes this line, we are sort of in the new process.
//SBO CODE END
	xor     %eax, %eax
	ret
