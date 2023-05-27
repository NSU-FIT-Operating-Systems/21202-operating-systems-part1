PROGRAMS=hello hello_wrapped_sc hello_nosc hello_addr_sc

build:
	for name in $(PROGRAMS); do\
		gcc -Wall $$name.c -o $$name;\
	done
	