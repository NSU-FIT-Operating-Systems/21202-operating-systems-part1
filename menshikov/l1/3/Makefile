CC=gcc
CFLAGS=-c -Wall


DYNAMIC_SRC=dynamicHello.c
DYNAMIC_OBJ=libdynamicHello.so
DYNFLAGS=-shared -fpic

EXECUTABLE=hello3
SOURCES=hello3.c
OBJECTS=$(SOURCES:.c=.o)

all: run

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
	
