	.section	__TEXT,__text,regular,pure_instructions
	.globl	_alou                           ## -- Begin function alou
_alou:                                  ## @alou
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
