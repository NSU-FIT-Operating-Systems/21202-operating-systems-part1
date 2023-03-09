CC=gcc
CFLAGS=-c -Wall

EXECUTABLE=hello
SOURCES=hello.c
OBJECTS=$(SOURCES:.c=.o)

DYNAMIC_SRC=hello-dyn-run.c
DYNAMIC_OBJ=libhello-dyn-run.so
DYNFLAGS=-shared -fpic

all: $(DYNAMIC_OBJ) $(EXECUTABLE)

$(DYNAMIC_OBJ): $(DYNAMIC_SRC)
	$(CC) $(DYNFLAGS) $(DYNAMIC_SRC) -o $(DYNAMIC_OBJ)
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) -ldl

run: all
	LD_LIBRARY_PATH=`pwd`&& export LD_LIBRARY_PATH && ./$(EXECUTABLE) && unset LD_LIBRARY_PATH;
clean:
	rm -rf *.o *.so $(EXECUTABLE) 
	