	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	gcd
	.type	gcd, @function
#gcd(a,b)=gcd(b%a,a)
gcd:
	pushq %rbp
	movq %rsp, %rbp
	cmpq %rdi, %rsi
	ja .GCD_FLIPPED
	xchgq %rdi, %rsi
.GCD_FLIPPED:
	testq %rdi, %rdi
	jz .GCD_ZERO
	xorq %rdx, %rdx
	movq %rsi, %rax
	divq %rdi
	movq %rdx, %rsi
	call gcd
	popq %rbp
	ret
.GCD_ZERO:
	movq %rsi, %rax
	popq %rbp
	ret
	.size	gcd, .-gcd
	.globl	lcm
	.type	lcm, @function
#lcm(a,b)=a*b/gcd(a,b)
lcm:
	pushq %rbp
	movq %rdi, %rax
	mulq %rsi
	movq %rax, %rcx
	call gcd
	xchgq %rax, %rcx
	xorq %rdx, %rdx
	divq %rcx
	popq %rbp
	ret
	.size	lcm, .-lcm
	.globl	main
	.type	main, @function
main:
	pushq %rbp
	movq %rsp, %rbp
	movq $1, %r8
	movq $1, %r9
.LOOP_LCM:
	movq %r8, %rdi
	movq %r9, %rsi
	call lcm
	movq %rax, %r8
	incq %r9
	cmpq $20, %r9
	jbe .LOOP_LCM
	movq %r8, %rsi
	movq $.LC0, %rdi
	call printf
	xorl %eax, %eax
	popq %rbp
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
