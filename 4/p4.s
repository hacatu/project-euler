	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq %rbp
	xorq %rsi, %rsi
	movq $100, %rbx
	movq $10, %r10
.LOOP_OUTER_FACTOR:
	cmpq $999, %rbx
	ja .DONE_OUTER_FACTOR
	movq %rbx, %rcx
.LOOP_INNER_FACTOR:
	cmpq $999, %rcx
	ja .DONE_INNER_FACTOR
	movq %rbx, %rax
	mulq %rcx
	xorq %rdi, %rdi
	cmpq %rax, %rsi
	ja .CONTINUE_INNER_FACTOR
	movq %rax, %r11
	movq %rax, %r8
.LOOP_PALINDROME_DIV:
	movq %r8, %rax
	xorq %rdx, %rdx
	divq %r10
	movq %rax, %r8
	movq %rdx, %r9
	movq %rdi, %rax
	mulq %r10
	addq %r9, %rax
	movq %rax, %rdi
	testq %r8, %r8
	jnz .LOOP_PALINDROME_DIV
	cmpq %r11, %rdi
	jne .CONTINUE_INNER_FACTOR
	movq %r11, %rsi
.CONTINUE_INNER_FACTOR:
	incq %rcx
	jmp .LOOP_INNER_FACTOR
.DONE_INNER_FACTOR:
	incq %rbx
	jmp .LOOP_OUTER_FACTOR
.DONE_OUTER_FACTOR:
	movq $.LC0, %rdi
	call printf
	xorl %eax, %eax
	popq %rbp
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
