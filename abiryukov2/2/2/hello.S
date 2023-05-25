.data
msg:
    .ascii "Hello, world\n"
    len = . - msg;

.text
    .global _start
_start:
    movq $1, %rax       # номер вызова sys_write
    movq $1, %rdi       # file descriptor (stdout)
    movq $msg, %rsi     # адрес строки
    movq $len, %rdx     # длина строки
    syscall

    movq $60, %rax      # номер системного вызова для завершения программы
    xorq %rdi, %rdi     # код возврата 0
    syscall
