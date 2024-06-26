# change application name here (executable output name)
TARGET=app
 
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread

RAYLIB_DIR=raylib/src
RAYLIB_FLAGS=-I$(RAYLIB_DIR) -L$(RAYLIB_DIR) -lraylib -lGL -lm -lpthread -ldl
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) $(RAYLIB_FLAGS) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) $(RAYLIB_FLAGS) -Wl,-rpath=$(RAYLIB_DIR) -export-dynamic
 
OBJS=    main.o arvore.o renderer.o
 
all: $(OBJS)
	$(LD) $(CCFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: src/main.c
	$(CC) $(CCFLAGS) -c src/main.c $(GTKLIB) -o main.o

renderer.o: src/renderer.c
	$(CC) $(CCFLAGS) -c src/renderer.c $(GTKLIB) -o renderer.o
	
arvore.o: src/arvore.c
	$(CC) $(CCFLAGS) -c src/arvore.c $(GTKLIB) -o arvore.o
    
clean:
	rm -f *.o $(TARGET)
