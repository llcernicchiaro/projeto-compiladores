## FILE_INIT
	.file	"output.txt"
	.text

.percentInt:
	.string	"%d"

.percentChar:
	.string	"%c"

.percentFloat:
	.string	"%f"
## TAC_BEGIN_FUN
	.globl sal
sal:
	pushq	%rbp
	movq	%rsp, %rbp
## TAC_ADD
	movl	_a(%rip), %edx
	movl	_5(%rip), %eax
	addl	%edx, %eax
	movl	%eax, _mYWeeirdT_emp_0(%rip)
## TAC_PRINT
	movl	_mYWeeirdT_emp_0(%rip), %eax
	movl	%eax, %esi
	leaq	.percentInt(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
## TAC_RETURN
	movl	_a(%rip), %eax
	popq	%rbp
	ret
## TAC_END_FUN
	movl	$0, %eax
	popq	%rb