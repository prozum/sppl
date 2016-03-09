.data
fmt:
	.string "%d\n"
.text
.globl main
.globl fac

main:
	movl $10000000000, %eax	# target - depth
	movl %eax, %ebx
	movl $0, %ecx	# result
	
	call fac

	pushl %ecx
	pushl $fmt
	call printf

	movl $0, %ebx
	movl $1, %eax
	int $0x80

fac:
	pushl %ebp
	movl %esp, %ebp
.facstart:
	cmpl $0, %eax
	jne .default
	jmp .case1
.case1:
	movl $1, %ecx
	jmp .out
.default:
	subl $1, %eax
	jmp .facstart
.out:
	cmpl %eax, %ebx
	jne .outsub
	jmp .finish
.outsub:
	addl $1, %eax
	imull %eax, %ecx
	jmp .out
.finish:
	leave
	ret
