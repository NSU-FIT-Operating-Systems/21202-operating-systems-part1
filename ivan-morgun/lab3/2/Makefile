APP=main
FUNCTIONS=mkd rmd lsd tch ctf rmf mksmlk ctsmlk rmsmlk mkhrdlk rmhrdlk lslf chmd

build:
	gcc -O3 $(APP).c -o $(APP).o

links: build
	for name in $(FUNCTIONS); do\
		ln $(APP).o $$name;\
	done

clear:
	rm $(APP).o $(FUNCTIONS)
