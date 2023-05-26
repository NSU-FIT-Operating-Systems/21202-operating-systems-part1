all:
	@echo "================ Код программы ================"
	cat hello_syscall.c

	@echo "================ Получение исполняемого файла ================"
	gcc hello_syscall.c -o hello_syscall.o

	@echo "================ Запуск исполняемого файла под strace ================"
	strace ./hello_syscall.o
