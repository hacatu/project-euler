	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq %rbp
	xorq %rsi, %rsi
	movq $1, %rdi
	movq $2, %rcx
.FIBONACCI_LOOP:
#f(n+3)=3f(n)+2f(n-1)
#f(n+2)=2f(n)+1f(n-1)
	addq %rcx, %rsi
	leaq (%rdi, %rcx, 2), %r8
	shlq $1, %rdi
	leaq (%rdi, %rcx, 2), %rdi
	addq %rcx, %rdi
	cmpq $4000000, %rdi
	jae .BREAK_FIBONACCI_LOOP
	addq %rdi, %rsi
	leaq (%r8, %rdi, 2), %rcx
	shlq $1, %r8
	leaq (%r8, %rdi, 2), %r8
	addq %rdi, %r8
	cmpq $4000000, %r8
	jae .BREAK_FIBONACCI_LOOP
	addq %r8, %rsi
	leaq (%rcx, %r8, 2), %rdi
	shlq $1, %rcx
	leaq (%rcx, %r8, 2), %rcx
	addq %r8, %rcx
	cmpq $4000000, %rcx
	jae .BREAK_FIBONACCI_LOOP
	jmp .FIBONACCI_LOOP
.BREAK_FIBONACCI_LOOP:
	movq $.LC0, %rdi
	call printf
	xorq %rax, %rax
	popq %rbp
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
