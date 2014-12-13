# Michael Newman && Mark Conley
# makefile for malloc & free
# Makes an executable called malloc that runs malloc.c, compiles sorted list files into a library

CC = gcc
CFLAGS = -Wall -g
OUTPUT = ./malloc
EXEOBJ = ./malloc.o
HEADERS = ./malloc.h
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSL): $(LIBOBJ)
	ar rc $(LIBSL) $(LIBOBJ)

$(OUTPUT): $(EXEOBJ) 
	$(CC) $(CFLAGS) $(EXEOBJ) malloc.h -o $@

clean:
	rm -f *.o $(OUTPUT)
