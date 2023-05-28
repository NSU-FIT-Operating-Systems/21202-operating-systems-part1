hello2: hello2.o libstaticHello.a
	gcc -o hello2 hello2.o -L. -lstaticHello

hello2.o: hello2.c
	gcc -c hello2.c

libstaticHello.a: staticHello.o
	ar cr libstaticHello.a staticHello.o

staticHello.o: staticHello.c
	gcc -c staticHello.c

clean:
	rm -f *.o *.a hello2
