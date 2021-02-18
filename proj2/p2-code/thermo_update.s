	.file	"thermo_update.c"
	.text
	.globl	set_temp_from_ports
	.type	set_temp_from_ports, @function
set_temp_from_ports:
.LFB23:
	.cfi_startproc
	endbr64
	movzwl	THERMO_SENSOR_PORT(%rip), %edx
	cmpw	$-1536, %dx
	ja	.L6
	movl	%edx, %ecx
	shrw	$6, %cx
	movzwl	%cx, %ecx
	leal	-500(%rcx), %eax
	testb	$32, %dl
	je	.L3
	leal	-499(%rcx), %eax
.L3:
	testb	$1, THERMO_STATUS_PORT(%rip)
	je	.L4
	leal	(%rax,%rax,8), %edx
	movslq	%edx, %rax
	imulq	$1717986919, %rax, %rax
	sarq	$33, %rax
	sarl	$31, %edx
	subl	%edx, %eax
	addl	$320, %eax
.L4:
	movw	%ax, (%rdi)
	testb	$1, THERMO_STATUS_PORT(%rip)
	jne	.L5
	movb	$0, 2(%rdi)
	movl	$0, %eax
	ret
.L5:
	movb	$1, 2(%rdi)
	movl	$0, %eax
	ret
.L6:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE23:
	.size	set_temp_from_ports, .-set_temp_from_ports
	.globl	set_display_from_temp
	.type	set_display_from_temp, @function
set_display_from_temp:
.LFB24:
	.cfi_startproc
	endbr64
	movl	%edi, %r8d
	sall	$8, %r8d
	sarl	$24, %r8d
	cmpb	$1, %r8b
	je	.L20
	testb	%r8b, %r8b
	jne	.L17
	leal	500(%rdi), %eax
	cmpw	$1000, %ax
	ja	.L18
.L10:
	movswl	%di, %edx
	testw	%di, %di
	js	.L21
	movl	$0, %r9d
.L11:
	movslq	%edx, %rax
	imulq	$1717986919, %rax, %rax
	sarq	$34, %rax
	movl	%edx, %ecx
	sarl	$31, %ecx
	subl	%ecx, %eax
	movl	%eax, %ecx
	leal	(%rax,%rax,4), %eax
	addl	%eax, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movslq	%ecx, %rdx
	imulq	$1717986919, %rdx, %rdx
	sarq	$34, %rdx
	movl	%ecx, %edi
	sarl	$31, %edi
	subl	%edi, %edx
	movl	%edx, %edi
	leal	(%rdx,%rdx,4), %edx
	addl	%edx, %edx
	subl	%edx, %ecx
	movl	%ecx, %edx
	movslq	%edi, %rcx
	imulq	$1717986919, %rcx, %rcx
	sarq	$34, %rcx
	movl	%edi, %r10d
	sarl	$31, %r10d
	subl	%r10d, %ecx
	movl	%ecx, %r10d
	leal	(%rcx,%rcx,4), %ecx
	addl	%ecx, %ecx
	subl	%ecx, %edi
	movl	%edi, %ecx
	movslq	%r10d, %rdi
	imulq	$1717986919, %rdi, %rdi
	sarq	$34, %rdi
	movl	%r10d, %r11d
	sarl	$31, %r11d
	subl	%r11d, %edi
	leal	(%rdi,%rdi,4), %edi
	addl	%edi, %edi
	subl	%edi, %r10d
	movl	%r10d, %edi
	je	.L12
	movslq	%r10d, %r10
	leaq	digit_arr(%rip), %r11
	orl	(%r11,%r10,4), %r9d
	sall	$7, %r9d
.L12:
	orl	%ecx, %edi
	je	.L13
	movslq	%ecx, %rcx
	leaq	digit_arr(%rip), %rdi
	orl	(%rdi,%rcx,4), %r9d
	sall	$7, %r9d
.L13:
	leaq	digit_arr(%rip), %rdi
	movslq	%edx, %rcx
	movl	%r9d, %edx
	orl	(%rdi,%rcx,4), %edx
	sall	$7, %edx
	cltq
	orl	(%rdi,%rax,4), %edx
	testb	%r8b, %r8b
	jne	.L14
	orl	$268435456, %edx
.L15:
	movl	%edx, (%rsi)
	movl	$0, %eax
	ret
.L20:
	leal	1220(%rdi), %eax
	cmpw	$2440, %ax
	jbe	.L10
	movl	$1, %eax
	ret
.L21:
	negl	%edx
	movl	$128, %r9d
	jmp	.L11
.L14:
	orl	$536870912, %edx
	jmp	.L15
.L17:
	movl	$1, %eax
	ret
.L18:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE24:
	.size	set_display_from_temp, .-set_display_from_temp
	.globl	thermo_update
	.type	thermo_update, @function
thermo_update:
.LFB25:
	.cfi_startproc
	endbr64
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movl	$0, 4(%rsp)
	movl	$0, (%rsp)
	leaq	4(%rsp), %rdi
	call	set_temp_from_ports
	cmpl	$1, %eax
	je	.L22
	movq	%rsp, %rsi
	movl	4(%rsp), %edi
	call	set_display_from_temp
	cmpl	$1, %eax
	je	.L22
	movl	(%rsp), %eax
	movl	%eax, THERMO_DISPLAY_PORT(%rip)
	movl	$0, %eax
.L22:
	movq	8(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L26
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L26:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE25:
	.size	thermo_update, .-thermo_update
	.globl	digit_arr
	.data
	.align 32
	.type	digit_arr, @object
	.size	digit_arr, 40
digit_arr:
	.long	126
	.long	12
	.long	55
	.long	31
	.long	77
	.long	91
	.long	123
	.long	14
	.long	127
	.long	95
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
