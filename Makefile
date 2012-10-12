# Leonardo Tonetto - Simple makefile to compile the tool.c
CC = gcc -Wall -O2

CCOPTS = -I./include -I/usr/include -c
LDOPTS = -L/usr/lib/ -lfreeimage

SOURCES = src/tool.c
OBJECTS = obj/tool.o
TARGET  = bin/tool

bin/tool: $(OBJECTS)
	$(CC) $(LDOPTS) $(OBJECTS) -o $(TARGET)

obj/tool.o:
	$(CC) $(CCOPTS) $(SOURCES) -o $(OBJECTS)

default: $(TARGET)

clean:
	/bin/rm -f obj/*.o $(TARGET)