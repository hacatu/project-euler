	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq %rbp
	movq $600851475143, %rsi
	bsfq %rsi, %rcx
	shlq %cl, %rsi
	movq $3, %rdi
	xorq %rdx, %rdx
.FACTOR_ODD:
	movq %rdi, %rax
	mulq %rax
	cmpq %rsi, %rax
	ja .DONE_FACTORING
	movq %rsi, %rax
	divq %rdi
	testq %rdx, %rdx
	jz .DIVIDE_ONCE
	addq $2, %rdi
	jmp .FACTOR_ODD
.DIVIDE_ONCE:
	movq %rax, %rsi
	jmp .FACTOR_ODD
.DONE_FACTORING:
	movq $.LC0, %rdi
	call printf
	xorl %eax, %eax
	popq %rbp
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
