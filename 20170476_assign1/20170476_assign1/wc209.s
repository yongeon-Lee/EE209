	.file	"wc209.c"
	.section	.rodata
.LC0:
	.string	"%d %d %d\n"
	.align 8
.LC1:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, -32(%rbp)
	movl	$0, -28(%rbp)
	movl	$1, -24(%rbp)
	movl	$0, -20(%rbp)
	movl	$0, -16(%rbp)
	movl	$0, -4(%rbp)
	call	getchar
	movl	%eax, -12(%rbp)
	call	getchar
	movl	%eax, -8(%rbp)
	cmpl	$-1, -12(%rbp)
	jne	.L2
	movl	$0, -24(%rbp)
	movl	-28(%rbp), %ecx
	movl	-20(%rbp), %edx
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	jmp	.L3
.L2:
	cmpl	$47, -12(%rbp)
	jne	.L4
	cmpl	$42, -8(%rbp)
	jne	.L4
	movl	$0, -32(%rbp)
	jmp	.L5
.L4:
	addl	$1, -28(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L6
	movl	$0, -16(%rbp)
	jmp	.L7
.L6:
	movl	$1, -16(%rbp)
.L7:
	cmpl	$10, -12(%rbp)
	jne	.L8
	addl	$1, -24(%rbp)
.L8:
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	$1, -32(%rbp)
.L5:
	jmp	.L9
.L21:
	movl	-32(%rbp), %eax
	testl	%eax, %eax
	je	.L10
	cmpl	$1, %eax
	je	.L11
	jmp	.L9
.L10:
	cmpl	$1, -16(%rbp)
	jne	.L12
	movl	-16(%rbp), %eax
	addl	%eax, -20(%rbp)
	movl	$0, -16(%rbp)
.L12:
	movl	-24(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	call	getchar
	movl	%eax, -8(%rbp)
	jmp	.L13
.L15:
	cmpl	$10, -12(%rbp)
	jne	.L14
	addl	$1, -24(%rbp)
	addl	$1, -28(%rbp)
.L14:
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	call	getchar
	movl	%eax, -8(%rbp)
	cmpl	$-1, -8(%rbp)
	jne	.L13
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, %eax
	jmp	.L3
.L13:
	cmpl	$42, -12(%rbp)
	jne	.L15
	cmpl	$47, -8(%rbp)
	jne	.L15
	addl	$1, -28(%rbp)
	call	getchar
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	$1, -32(%rbp)
	jmp	.L9
.L11:
	cmpl	$47, -12(%rbp)
	jne	.L16
	cmpl	$42, -8(%rbp)
	jne	.L16
	movl	$0, -32(%rbp)
	jmp	.L9
.L16:
	addl	$1, -28(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L17
	cmpl	$1, -16(%rbp)
	jne	.L19
	movl	-16(%rbp), %eax
	addl	%eax, -20(%rbp)
	movl	$0, -16(%rbp)
	jmp	.L19
.L17:
	movl	$1, -16(%rbp)
.L19:
	cmpl	$10, -12(%rbp)
	jne	.L20
	addl	$1, -24(%rbp)
.L20:
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	$1, -32(%rbp)
	nop
.L9:
	call	getchar
	movl	%eax, -8(%rbp)
	cmpl	$-1, -8(%rbp)
	jne	.L21
	cmpl	$-1, -12(%rbp)
	je	.L22
	addl	$1, -28(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L23
	cmpl	$1, -16(%rbp)
	jne	.L25
	movl	-16(%rbp), %eax
	addl	%eax, -20(%rbp)
	movl	$0, -16(%rbp)
	jmp	.L25
.L23:
	addl	$1, -20(%rbp)
.L25:
	cmpl	$10, -12(%rbp)
	jne	.L22
	addl	$1, -24(%rbp)
.L22:
	movl	-28(%rbp), %ecx
	movl	-20(%rbp), %edx
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
