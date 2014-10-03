# Michael Newman && Mark Conley
# makefile for sorted list
# Makes an executable called sl that runs main.c, compiles sorted list files into a library

CC = gcc
CFLAGS = -Wall -g -o
# LFLAGS = -L
OUTPUT = sl
EXEOBJ = main.o
LIBOBJ = sorted-list.o
LIBSL = libsl.a
all: $(OUTPUT)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

sorted-list.o : sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c -o $(LIBOBJ)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c -o $(EXEOBJ)

$(LIBSL): $(LIBOBJ)
	ar rc $(LIBSL) $(LIBOBJ)

$(OUTPUT): $(EXEOBJ) $(LIBSL)
	$(CC) $(CFLAGS) $(EXEOBJ) $(LIBSL) -o $(OUTPUT)

clean:
	rm *.o $(OUTPUT)