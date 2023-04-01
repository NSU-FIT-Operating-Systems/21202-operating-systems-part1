CC=gcc
CFLAGS=-Wall -O3
SOURCE=lab3-2.c
EXE=$(SOURCE:.c=.out)
COMMAND_NAMES=mkd rmd lsd crt fprint rmf smlk smlprint hrdlk fstt chmd

all: links

$(EXE): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXE)

links: $(EXE)
	for name in $(COMMAND_NAMES); do\
		ln $(EXE) $$name;\
	done

clean:
	rm -rf *.out $(COMMAND_NAMES)
