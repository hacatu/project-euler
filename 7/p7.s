	.section	.rodata
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$10001, %rdi
	movq	$500000, %rbx
	subq	%rbx, %rsp
	movq	%rbx, %rcx
.LOOP_ARRAY:
	movb	$1, -1(%rsp,%rcx)
	loopq	.LOOP_ARRAY
	movq	$2, %rcx
.LOOP_SEIVE:
	testb	$1, (%rsp,%rcx)
	jz		.CONT_SEIVE
	movq	%rcx, %rax
	mulq	%rcx
	cmpq	%rbx, %rax
	ja		.DONE_SEIVE
.LOOP_FACTORS:
	movb	$0, (%rsp,%rax)
	addq	%rcx, %rax
	cmpq	%rbx, %rax
	jbe		.LOOP_FACTORS
.CONT_SEIVE:
	incq	%rcx
	jmp		.LOOP_SEIVE
.DONE_SEIVE:
	movq	%rdi, %rcx
	movq	$1, %rsi
.FIND_PRIMES:
	incq	%rsi
	testb	$1, (%rsp,%rsi)
	jz		.FIND_PRIMES
	loopq	.FIND_PRIMES
	movq	$.LC0, %rdi
	call	printf
	xorq	%rax, %rax
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
