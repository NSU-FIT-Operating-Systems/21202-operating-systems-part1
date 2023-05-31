.data

msg:
	.ascii "Hello world from syscall\n"
	len = . - msg;
.text
	.global main

main:
	movq $1, %rax
	movq $1, %rdi
	movq $msg, %rsi
	movq $len, %rdx
	syscall

	movq $60, %rax
	xorq %rdi, %rdi
	syscall
