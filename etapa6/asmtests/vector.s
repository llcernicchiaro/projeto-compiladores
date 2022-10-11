	.file	"vector.c"
	.text
	.globl	sal
sal:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

	movl	$5, 12+sal(%rip)
	
	movl	$0, %eax
	popq	%rbp
	ret

0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
