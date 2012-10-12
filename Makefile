# Leonardo Tonetto - Simple makefile to compile the tool.c
CC = gcc -Wall -O2

CCOPTS = -I./include -I/usr/include -c
LDOPTS = -L/usr/lib/ -lfreeimage

OBJDIR = ./obj
BINDIR = ./bin
SRCDIR = ./src

SOURCES = $(SRCDIR)/tool.c
OBJECTS = $(OBJDIR)/tool.o
TARGET  = $(BINDIR)/tool

bin/tool: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $(LDOPTS) $(OBJECTS) -o $(TARGET)

obj/tool.o:
	mkdir -p $(OBJDIR)
	$(CC) $(CCOPTS) $(SOURCES) -o $(OBJECTS)

default: $(TARGET)

clean:
	/bin/rm -f obj/*.o $(TARGET)