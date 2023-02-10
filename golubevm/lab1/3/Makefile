CC=gcc
CFLAGS=-c -Wall


DYNAMIC_SRC=hello_dyn.c
DYNAMIC_OBJ=libhello-dyn.so
DYNFLAGS=-shared -fpic

EXECUTABLE=hello
SOURCES=hello.c
OBJECTS=$(SOURCES:.c=.o)

all: addToPath run

dynamic: $(DYNAMIC_SRC)
	$(CC) $(DYNFLAGS) $(DYNAMIC_SRC) -o $(DYNAMIC_OBJ)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

$(EXECUTABLE): $(OBJECTS) dynamic
	$(CC) $(OBJECTS) $(DYNAMIC_OBJ) -L. -Wl,-rpath=$$(pwd) -o $@
	
clean:
	rm -rf *.o *.so $(EXECUTABLE) 
	
run: $(EXECUTABLE)
	./$(EXECUTABLE)
	
addToPath:
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH