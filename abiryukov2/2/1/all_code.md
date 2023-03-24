all_code.md:
```

```
a/ii/hello_write.c:
```
#include <unistd.h>

int main() {
  write(STDOUT_FILENO, "Hello world\n", 13);
  return 0;
}

```
a/ii/hello_write.o:
```
a/ii/Makefile:
```
all:
	@echo "================ Код программы ================"
	cat hello_write.c

	@echo "================ Получение исполняемого файла ================"
	gcc hello_write.c -o hello_write.o

	@echo "================ Запуск исполняемого файла под strace ================"
	strace ./hello_write.o

```
a/iii/hello_syscall.c:
```
#include <unistd.h>

void print_hello() {
  write(STDOUT_FILENO, "Hello world\n", 13);
}

int main() {
  print_hello();
  return 0;
}

```
a/iii/hello_syscall.o:
```
a/iii/Makefile:
```
all:
	@echo "================ Код программы ================"
	cat hello_syscall.c

	@echo "================ Получение исполняемого файла ================"
	gcc hello_syscall.c -o hello_syscall.o

	@echo "================ Запуск исполняемого файла под strace ================"
	strace ./hello_syscall.o

```
a/i/hello.c:
```
#include <stdio.h>

int main() {
    printf("Hello world\n");
}

```
a/i/Makefile:
```
all:
	@echo "================ Получение исполняемого файла ================"
	gcc hello.c -o hello.o

	@echo "================ Запуск исполняемого файла под strace ================"
	strace ./hello.o

	@echo -e "\n================================\nПосле запуска программы под strace, мы можем увидеть список системных вызовов, которые выполняет программа. Как правило, первыми вызовами будут открытие файла, загрузка библиотек и т.д. Затем, в конце списка, мы должны увидеть системный вызов \"write\", который используется для вывода \"Hello world\" в консоль.\n================================"

```
b/Makefile:
```
strace -c wget kernel.org

```
