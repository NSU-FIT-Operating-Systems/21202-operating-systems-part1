all:
	@echo "1. Проведите следующие эксперименты:"
	@$(MAKE) --no-print-directory --always-make a
	@$(MAKE) --no-print-directory --always-make b

a:
	@echo "	a. запустите программу hello world из предыдущей задачи под strace:"
	@$(MAKE) --no-print-directory --always-make a-i
	@$(MAKE) --no-print-directory --always-make a-ii
	@$(MAKE) --no-print-directory --always-make a-iii

a-i:
	@echo "		i. обратите внимание какие системные вызовы были вызваны в   процессе исполнения программы. Чем обусловлено такое   количество системных вызовов. Какой системный вызов   используется для вывода “hello world”? Изучите этот вызов и   разберитесь что он принимает и возвращает."
	cd a/i && make
	@echo "\n\n\n"

a-ii:
	@echo "		ii. используйте этот сискол в программе hello world вместо printf().   Убедитесь что этот вызов присутствует в выводе strace."
	cd a/ii && make
	@echo "\n\n\n"

a-iii:
	@echo "		iii. напишите свою обертку над этим сисколом. Для этого   используйте функцию syscall() из libc. Также проверьте вывод   strace."
	cd a/iii && make
	@echo "\n\n\n"

b:
	@echo "	b. Запустите под strace команду ‘wget kernel.org’ (если нет wget,   используйте curl). Получите статистику использования системных   вызовов порожденным процессом."
	cd b && make
	@echo "\n\n\n"
