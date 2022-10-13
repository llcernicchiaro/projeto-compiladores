## FILE_INIT
	.file	"output.txt"
	.text

.percentInt:
	.string	"%d"

.percentChar:
	.string	"%c"

.percentFloat:
	.string	"%f"
## TAC_VAR_DECLARATION
## TAC_BEGIN_FUN
	.globl main
main:
	pushq	%rbp
	movq	%rsp, %rbp
## TAC_OR
## TAC_PRINT
	movl	_mYWeeirdT_emp_0(%rip), %eax
	movl	%eax, %esi
	leaq	.percentInt(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
## TAC_END_FUN
	movl	$0, %eax
	popq	%rbp
	ret
## DATA_SECTION
.section	.data
_mYWeeirdT_emp_1:
	.long	
_10:
	.long	10
_variable:
	.long	10
_mYWeeirdT_emp_0:
	.long	
## FILE_END
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
