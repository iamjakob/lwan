	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB0:
	.text
LHOTB0:
	.align 4,0x90
	.globl _init_queue
_init_queue:
LFB4:
	pushq	%rbp
LCFI0:
	movq	%rsi, %rbp
	movl	$8, %esi
	pushq	%rbx
LCFI1:
	movq	%rdi, %rbx
	movq	%rbp, %rdi
	subq	$8, %rsp
LCFI2:
	call	_calloc
	movq	%rbp, 80(%rbx)
	movq	%rax, 88(%rbx)
	movq	%rax, 24(%rbx)
	xorl	%eax, %eax
	movq	%rbp, 16(%rbx)
	movq	%rax, (%rbx)
	mfence
	movq	%rax, 64(%rbx)
	mfence
	movq	$0, 8(%rbx)
	movq	$0, 72(%rbx)
	addq	$8, %rsp
LCFI3:
	popq	%rbx
LCFI4:
	popq	%rbp
LCFI5:
	ret
LFE4:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE0:
	.text
LHOTE0:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB1:
	.text
LHOTB1:
	.align 4,0x90
	.globl _clear_queue
_clear_queue:
LFB5:
	pushq	%rbx
LCFI6:
	movq	%rdi, %rbx
	movq	88(%rdi), %rdi
	call	_free
	movl	$3735928559, %eax
	movq	%rax, (%rbx)
	mfence
	movq	%rax, 64(%rbx)
	mfence
	movq	%rax, 8(%rbx)
	movq	%rax, 72(%rbx)
	movq	%rax, 24(%rbx)
	movq	%rax, 88(%rbx)
	popq	%rbx
LCFI7:
	ret
LFE5:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE1:
	.text
LHOTE1:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB2:
	.text
LHOTB2:
	.align 4,0x90
	.globl _push_many
_push_many:
LFB6:
	movq	64(%rdi), %rcx
	movq	80(%rdi), %r8
	movq	%r8, %rax
	addq	72(%rdi), %rax
	subq	%rcx, %rax
	cmpq	%rdx, %rax
	jb	L6
	testq	%rdx, %rdx
	movq	88(%rdi), %r10
	movq	%rcx, %r9
	je	L8
L7:
	leaq	(%rcx,%rdx), %r9
	movq	%rcx, %rax
	negq	%rax
	leaq	(%rsi,%rax,8), %r11
	.align 4,0x90
L11:
	movq	(%r11,%rcx,8), %rsi
	movq	%rcx, %rax
	xorl	%edx, %edx
	addq	$1, %rcx
	divq	%r8
	cmpq	%rcx, %r9
	movq	%rsi, (%r10,%rdx,8)
	jne	L11
L8:
	movq	%r9, 64(%rdi)
	movl	$1, %eax
	ret
	.align 4,0x90
L6:
	movq	(%rdi), %rax
	movq	%r8, %r9
	subq	%rcx, %r9
	addq	%rax, %r9
	movq	%rax, 72(%rdi)
	xorl	%eax, %eax
	cmpq	%r9, %rdx
	ja	L17
	movq	88(%rdi), %r10
	jmp	L7
	.align 4,0x90
L17:
	ret
LFE6:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE2:
	.text
LHOTE2:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB3:
	.text
LHOTB3:
	.align 4,0x90
	.globl _push
_push:
LFB7:
	subq	$8, %rsp
LCFI8:
	movl	$1, %edx
	movq	%rsi, (%rsp)
	movq	%rsp, %rsi
	call	_push_many
	addq	$8, %rsp
LCFI9:
	ret
LFE7:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE3:
	.text
LHOTE3:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB4:
	.text
LHOTB4:
	.align 4,0x90
	.globl _pop_many
_pop_many:
LFB8:
	movq	(%rdi), %rcx
	movq	8(%rdi), %rax
	movq	80(%rdi), %r8
	subq	%rcx, %rax
	cmpq	%rdx, %rax
	jb	L21
	testq	%rdx, %rdx
	movq	24(%rdi), %r10
	movq	%rcx, %r9
	je	L23
L22:
	leaq	(%rcx,%rdx), %r9
	movq	%rcx, %rax
	negq	%rax
	leaq	(%rsi,%rax,8), %rsi
	.align 4,0x90
L26:
	movq	%rcx, %rax
	xorl	%edx, %edx
	divq	%r8
	movq	(%r10,%rdx,8), %rax
	movq	%rax, (%rsi,%rcx,8)
	addq	$1, %rcx
	cmpq	%rcx, %r9
	jne	L26
L23:
	movq	%r9, (%rdi)
	movl	$1, %eax
	ret
	.align 4,0x90
L21:
	movq	64(%rdi), %rax
	movq	%rax, 8(%rdi)
	subq	%rcx, %rax
	movq	%rax, %r9
	xorl	%eax, %eax
	cmpq	%r9, %rdx
	ja	L31
	movq	24(%rdi), %r10
	jmp	L22
	.align 4,0x90
L31:
	ret
LFE8:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE4:
	.text
LHOTE4:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB5:
	.text
LHOTB5:
	.align 4,0x90
	.globl _pop
_pop:
LFB9:
	movl	$1, %edx
	jmp	_pop_many
LFE9:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE5:
	.text
LHOTE5:
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0
	.byte	0x1
	.ascii "zR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x1
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB4-.
	.set L$set$2,LFE4-LFB4
	.quad L$set$2
	.byte	0
	.byte	0x4
	.set L$set$3,LCFI0-LFB4
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xe
	.byte	0x18
	.byte	0x83
	.byte	0x3
	.byte	0x4
	.set L$set$5,LCFI2-LCFI1
	.long L$set$5
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$6,LCFI3-LCFI2
	.long L$set$6
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$7,LCFI4-LCFI3
	.long L$set$7
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$8,LCFI5-LCFI4
	.long L$set$8
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE1:
LSFDE3:
	.set L$set$9,LEFDE3-LASFDE3
	.long L$set$9
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB5-.
	.set L$set$10,LFE5-LFB5
	.quad L$set$10
	.byte	0
	.byte	0x4
	.set L$set$11,LCFI6-LFB5
	.long L$set$11
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$12,LCFI7-LCFI6
	.long L$set$12
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE3:
LSFDE5:
	.set L$set$13,LEFDE5-LASFDE5
	.long L$set$13
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB6-.
	.set L$set$14,LFE6-LFB6
	.quad L$set$14
	.byte	0
	.align 3
LEFDE5:
LSFDE7:
	.set L$set$15,LEFDE7-LASFDE7
	.long L$set$15
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB7-.
	.set L$set$16,LFE7-LFB7
	.quad L$set$16
	.byte	0
	.byte	0x4
	.set L$set$17,LCFI8-LFB7
	.long L$set$17
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$18,LCFI9-LCFI8
	.long L$set$18
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE7:
LSFDE9:
	.set L$set$19,LEFDE9-LASFDE9
	.long L$set$19
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB8-.
	.set L$set$20,LFE8-LFB8
	.quad L$set$20
	.byte	0
	.align 3
LEFDE9:
LSFDE11:
	.set L$set$21,LEFDE11-LASFDE11
	.long L$set$21
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB9-.
	.set L$set$22,LFE9-LFB9
	.quad L$set$22
	.byte	0
	.align 3
LEFDE11:
	.subsections_via_symbols
