NAME=hello

run:
	strace -o $(NAME).log ./$(NAME)
	cat $(NAME).log
	$(info ==========SYSTEM CALLS FROM $(NAME))
runwrite:
	gcc -Wall hello.c -o hellowrite
	strace -o hellowrite.log ./hellowrite
	cat hellowrite.log
	$(info =====SYSTEM CALLS FROM hellowrite)

runsys:
	gcc -Wall hellosys.c -o hellosys
	strace -o hellosys.log ./hellosys
	cat hellosys.log
	$(info =====SYSTEM CALLS FROM hellosys)

clean:
	rm *.log hello hellosys hellowrite