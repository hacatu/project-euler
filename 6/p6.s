	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
#sum(n^2)=n*(n+1)*(2*n+1)/6
#sum(n)^2=n^2*(n+1)^2/4
#sum(n)^2-sum(n^2)=n*(n-1)*(n+1)*(3*n+2)
	pushq %rbp
	movq $100, %rdi
	leaq 2(%rdi,%rdi,2), %rax
	leaq 1(%rdi), %rbx
	mulq %rbx
	leaq -1(%rdi), %rbx
	mulq %rbx
	mulq %rdi
	movq $12, %rbx
	divq %rbx
	movq %rax, %rsi
	movq $.LC0, %rdi
	call printf
	popq %rbp
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
