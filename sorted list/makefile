# Michael Newman && Mark Conley
# makefile for sorted list
# Makes an executable called sl that runs main.c, compiles sorted list files into a library

CC = gcc
CFLAGS = -Wall -g
OUTPUT = ./sl
EXEOBJ = ./main.o
LIBOBJ = ./sorted-list.o
LIBSL = ./libsl.a
HEADERS = ./*.h
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBSL): $(LIBOBJ)
	ar rc $(LIBSL) $(LIBOBJ)

$(OUTPUT): $(EXEOBJ) $(LIBSL)
	$(CC) $(CFLAGS) $(EXEOBJ) $(LIBSL) sorted-list.h -o $@

clean:
	rm -f *.o $(OUTPUT)
	rm -f $(LIBSL)