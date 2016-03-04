.data
fmt:
	.ascii "%d\n"		# format used for printing result

.text
.globl main
.globl fac

main:
	pushl $7		# target value (DONT CHANGE)
	pushl (%esp)		# copy of target value, can be changed
	pushl $0		# Return value goes here
	
	call fac		# call fac function

	pushl $fmt		# push format for print
	call printf		# print result

	movl $0, %ebx		# set status code to 0
	movl $1, %eax		# set system call to SYS_EXIT
	int $0x80

fac:
	pushl %ebp		# push base pointer
	movl %esp, %ebp		# set stack pointer

	movl 8(%esp), %eax	# return value
	movl 12(%esp), %ebx	# target value

	cmpl $0, %ebx		# checks if case0 requirement 0 is met
	jne .default		# if not, go to default case
	jmp .case1		# else, go to case 1
.case1:
	movl $1, 8(%esp)	# return 1
	jmp .finish		# finish function
.default:
	subl $1, %ebx
	pushl %ebx		# push target
	pushl %eax		# push return

	call fac		# recuesion

	popl %eax		# pop return
	popl %ebx		# pop target

	addl $1, %ebx		# increase target by 1
	imull %ebx, %eax	# multiply result and target
	movl %eax, 8(%esp)	# returns result
	jmp .finish		# finish function
.finish:
	leave			# prepare to leave (clean up)
	ret			# return to caller
