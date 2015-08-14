	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB0:
	.text
LHOTB0:
	.align 4,0x90
	.globl _do_pops
_do_pops:
LFB2:
	pushq	%r14
LCFI0:
	xorl	%r14d, %r14d
	pushq	%r13
LCFI1:
	pushq	%r12
LCFI2:
	pushq	%rbp
LCFI3:
	pushq	%rbx
LCFI4:
	xorl	%ebx, %ebx
	subq	$32, %rsp
LCFI5:
	movq	_shared_queue@GOTPCREL(%rip), %rbp
	movq	$0, 8(%rsp)
	leaq	8(%rsp), %r12
	leaq	16(%rsp), %r13
	.align 4,0x90
L2:
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	_pop
	testb	%al, %al
	je	L3
L9:
	movq	8(%rsp), %rax
	addq	$1, %rbx
	cmpq	%rax, %rbx
	je	L4
	movl	$0, _isgood(%rip)
L4:
	cmpq	$100000000, %rax
	je	L5
	movq	%r12, %rsi
	movq	%rbp, %rdi
	xorl	%r14d, %r14d
	call	_pop
	testb	%al, %al
	jne	L9
L3:
	addq	$10, %r14
	xorl	%esi, %esi
	movq	%r13, %rdi
	movq	$0, 16(%rsp)
	imulq	$1000, %r14, %rax
	movq	%rax, 24(%rsp)
	call	_nanosleep
	jmp	L2
	.align 4,0x90
L5:
	addq	$32, %rsp
LCFI6:
	xorl	%eax, %eax
	popq	%rbx
LCFI7:
	popq	%rbp
LCFI8:
	popq	%r12
LCFI9:
	popq	%r13
LCFI10:
	popq	%r14
LCFI11:
	ret
LFE2:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE0:
	.text
LHOTE0:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB1:
	.text
LHOTB1:
	.align 4,0x90
	.globl _do_pushes
_do_pushes:
LFB3:
	pushq	%r13
LCFI12:
	pushq	%r12
LCFI13:
	pushq	%rbp
LCFI14:
	xorl	%ebp, %ebp
	pushq	%rbx
LCFI15:
	movl	$1, %ebx
	subq	$24, %rsp
LCFI16:
	movq	_shared_queue@GOTPCREL(%rip), %r12
	movq	%rsp, %r13
	jmp	L13
	.align 4,0x90
L17:
	addq	$1, %rbx
	xorl	%ebp, %ebp
	cmpq	$100000001, %rbx
	ja	L16
L13:
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	_push
	testb	%al, %al
	jne	L17
	addq	$10, %rbp
	xorl	%esi, %esi
	movq	%r13, %rdi
	movq	$0, (%rsp)
	imulq	$1000, %rbp, %rax
	movq	%rax, 8(%rsp)
	call	_nanosleep
	cmpq	$100000001, %rbx
	jbe	L13
L16:
	addq	$24, %rsp
LCFI17:
	xorl	%eax, %eax
	popq	%rbx
LCFI18:
	popq	%rbp
LCFI19:
	popq	%r12
LCFI20:
	popq	%r13
LCFI21:
	ret
LFE3:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE1:
	.text
LHOTE1:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB2:
	.text
LHOTB2:
	.align 4,0x90
	.globl _sleep_micros
_sleep_micros:
LFB1:
	imulq	$1000, %rdi, %rdi
	subq	$24, %rsp
LCFI22:
	xorl	%esi, %esi
	movq	$0, (%rsp)
	movq	%rdi, 8(%rsp)
	movq	%rsp, %rdi
	call	_nanosleep
	addq	$24, %rsp
LCFI23:
	ret
LFE1:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE2:
	.text
LHOTE2:
	.cstring
	.align 3
LC6:
	.ascii "Time spent was %f, %f pointers per second, %f mb/sec\12\0"
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB7:
	.text
LHOTB7:
	.align 4,0x90
	.globl _printtime
_printtime:
LFB4:
	pushq	%rbx
LCFI24:
	movq	%rdi, %rbx
	call	_clock
	subq	%rbx, %rax
	js	L21
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
L22:
	popq	%rbx
LCFI25:
	leaq	LC6(%rip), %rdi
	movl	$3, %eax
	movsd	LC3(%rip), %xmm3
	movsd	LC4(%rip), %xmm1
	movsd	LC5(%rip), %xmm2
	divsd	%xmm3, %xmm0
	divsd	%xmm0, %xmm1
	mulsd	%xmm1, %xmm2
	divsd	%xmm3, %xmm2
	jmp	_printf
	.align 4,0x90
L21:
LCFI26:
	movq	%rax, %rdx
	andl	$1, %eax
	pxor	%xmm0, %xmm0
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	L22
LFE4:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE7:
	.text
LHOTE7:
	.cstring
LC8:
	.ascii "Was the program valid? %d\12\0"
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB9:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTB9:
	.align 4
	.globl _main
_main:
LFB5:
	pushq	%rbp
LCFI27:
	movl	$5000, %esi
	pushq	%rbx
LCFI28:
	subq	$24, %rsp
LCFI29:
	movq	_shared_queue@GOTPCREL(%rip), %rbx
	movq	%rbx, %rdi
	call	_init_queue
	call	_clock
	movq	%rsp, %rdi
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	leaq	_do_pushes(%rip), %rdx
	movq	%rax, %rbp
	call	_pthread_create
	leaq	8(%rsp), %rdi
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	leaq	_do_pops(%rip), %rdx
	call	_pthread_create
	movq	(%rsp), %rdi
	xorl	%esi, %esi
	call	_pthread_join
	movq	8(%rsp), %rdi
	xorl	%esi, %esi
	call	_pthread_join
	movl	_isgood(%rip), %esi
	leaq	LC8(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
	movq	%rbp, %rdi
	call	_printtime
	movq	%rbx, %rdi
	call	_clear_queue
	addq	$24, %rsp
LCFI30:
	xorl	%eax, %eax
	popq	%rbx
LCFI31:
	popq	%rbp
LCFI32:
	ret
LFE5:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE9:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTE9:
	.globl _isgood
	.data
	.align 2
_isgood:
	.long	1
	.comm	_micro_slept,8,3
	.comm	_shared_queue,96,5
	.literal8
	.align 3
LC3:
	.long	0
	.long	1093567616
	.align 3
LC4:
	.long	0
	.long	1100470148
	.align 3
LC5:
	.long	0
	.long	1075838976
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
	.quad	LFB2-.
	.set L$set$2,LFE2-LFB2
	.quad L$set$2
	.byte	0
	.byte	0x4
	.set L$set$3,LCFI0-LFB2
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x8e
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xe
	.byte	0x18
	.byte	0x8d
	.byte	0x3
	.byte	0x4
	.set L$set$5,LCFI2-LCFI1
	.long L$set$5
	.byte	0xe
	.byte	0x20
	.byte	0x8c
	.byte	0x4
	.byte	0x4
	.set L$set$6,LCFI3-LCFI2
	.long L$set$6
	.byte	0xe
	.byte	0x28
	.byte	0x86
	.byte	0x5
	.byte	0x4
	.set L$set$7,LCFI4-LCFI3
	.long L$set$7
	.byte	0xe
	.byte	0x30
	.byte	0x83
	.byte	0x6
	.byte	0x4
	.set L$set$8,LCFI5-LCFI4
	.long L$set$8
	.byte	0xe
	.byte	0x50
	.byte	0x4
	.set L$set$9,LCFI6-LCFI5
	.long L$set$9
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$10,LCFI7-LCFI6
	.long L$set$10
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$11,LCFI8-LCFI7
	.long L$set$11
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$12,LCFI9-LCFI8
	.long L$set$12
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$13,LCFI10-LCFI9
	.long L$set$13
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$14,LCFI11-LCFI10
	.long L$set$14
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE1:
LSFDE3:
	.set L$set$15,LEFDE3-LASFDE3
	.long L$set$15
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB3-.
	.set L$set$16,LFE3-LFB3
	.quad L$set$16
	.byte	0
	.byte	0x4
	.set L$set$17,LCFI12-LFB3
	.long L$set$17
	.byte	0xe
	.byte	0x10
	.byte	0x8d
	.byte	0x2
	.byte	0x4
	.set L$set$18,LCFI13-LCFI12
	.long L$set$18
	.byte	0xe
	.byte	0x18
	.byte	0x8c
	.byte	0x3
	.byte	0x4
	.set L$set$19,LCFI14-LCFI13
	.long L$set$19
	.byte	0xe
	.byte	0x20
	.byte	0x86
	.byte	0x4
	.byte	0x4
	.set L$set$20,LCFI15-LCFI14
	.long L$set$20
	.byte	0xe
	.byte	0x28
	.byte	0x83
	.byte	0x5
	.byte	0x4
	.set L$set$21,LCFI16-LCFI15
	.long L$set$21
	.byte	0xe
	.byte	0x40
	.byte	0x4
	.set L$set$22,LCFI17-LCFI16
	.long L$set$22
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$23,LCFI18-LCFI17
	.long L$set$23
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$24,LCFI19-LCFI18
	.long L$set$24
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$25,LCFI20-LCFI19
	.long L$set$25
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$26,LCFI21-LCFI20
	.long L$set$26
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE3:
LSFDE5:
	.set L$set$27,LEFDE5-LASFDE5
	.long L$set$27
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB1-.
	.set L$set$28,LFE1-LFB1
	.quad L$set$28
	.byte	0
	.byte	0x4
	.set L$set$29,LCFI22-LFB1
	.long L$set$29
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$30,LCFI23-LCFI22
	.long L$set$30
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE5:
LSFDE7:
	.set L$set$31,LEFDE7-LASFDE7
	.long L$set$31
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB4-.
	.set L$set$32,LFE4-LFB4
	.quad L$set$32
	.byte	0
	.byte	0x4
	.set L$set$33,LCFI24-LFB4
	.long L$set$33
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$34,LCFI25-LCFI24
	.long L$set$34
	.byte	0xa
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$35,LCFI26-LCFI25
	.long L$set$35
	.byte	0xb
	.align 3
LEFDE7:
LSFDE9:
	.set L$set$36,LEFDE9-LASFDE9
	.long L$set$36
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB5-.
	.set L$set$37,LFE5-LFB5
	.quad L$set$37
	.byte	0
	.byte	0x4
	.set L$set$38,LCFI27-LFB5
	.long L$set$38
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$39,LCFI28-LCFI27
	.long L$set$39
	.byte	0xe
	.byte	0x18
	.byte	0x83
	.byte	0x3
	.byte	0x4
	.set L$set$40,LCFI29-LCFI28
	.long L$set$40
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$41,LCFI30-LCFI29
	.long L$set$41
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$42,LCFI31-LCFI30
	.long L$set$42
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$43,LCFI32-LCFI31
	.long L$set$43
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE9:
	.subsections_via_symbols
