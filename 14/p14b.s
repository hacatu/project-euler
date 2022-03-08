	.file	"p14b.c"
	.globl	collatzs
	.data
	.align 32
	.type	collatzs, @object
	.size	collatzs, 2313817346
collatzs:
	.zero	2
	.value	1
	.zero	2313817342
	.text
	.globl	collatz
	.type	collatz, @function
collatz:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movzwl	collatzs(%rax,%rax), %eax
	testw	%ax, %ax
	je	.L2
	movq	-8(%rbp), %rax
	movzwl	collatzs(%rax,%rax), %eax
	movzwl	%ax, %eax
	jmp	.L3
.L2:
	movq	-8(%rbp), %rax
	andl	$1, %eax
	testq	%rax, %rax
	je	.L4
	movq	-8(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	$1, %rax
	jmp	.L5
.L4:
	movq	-8(%rbp), %rax
	shrq	%rax
.L5:
	movq	%rax, %rdi
	call	collatz
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movw	%dx, collatzs(%rax,%rax)
	movq	-8(%rbp), %rax
	movzwl	collatzs(%rax,%rax), %eax
	movzwl	%ax, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	collatz, .-collatz
	.section	.rodata
.LC0:
	.string	"%uli\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	$0, -8(%rbp)
	movq	$500001, -24(%rbp)
	jmp	.L7
.L9:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	collatz
	movw	%ax, -26(%rbp)
	movzwl	-26(%rbp), %eax
	cmpq	-8(%rbp), %rax
	jbe	.L8
	movzwl	-26(%rbp), %eax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
.L8:
	addq	$2, -24(%rbp)
.L7:
	cmpq	$999999, -24(%rbp)
	jbe	.L9
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
