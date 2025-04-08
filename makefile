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

RAYLIB_DIR=/raylib
RAYLIB_FLAGS=-Iraylib -Lraylib -lraylib -lGL -lm -lpthread -ldl
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) $(RAYLIB_FLAGS) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) $(RAYLIB_FLAGS) -Wl,-rpath=$(RAYLIB_DIR) -export-dynamic
 
OBJS=    main.o arvore.o textinput.o
 
all: $(OBJS)
	$(LD) $(CCFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o: src/main.c
	$(CC) $(CCFLAGS) -c src/main.c $(GTKLIB) -o main.o

textinput.o: src/textinput.c
	$(CC) $(CCFLAGS) -c src/textinput.c $(GTKLIB) -o textinput.o
	
arvore.o: src/arvore.c
	$(CC) $(CCFLAGS) -c src/arvore.c $(GTKLIB) -o arvore.o
    
clean:
	rm -f *.o $(TARGET)
