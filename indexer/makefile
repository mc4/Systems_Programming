# Mark Conley && Michael Newman
# makefile for indexer
# Makes an executable called index

CC = gcc
CFLAGS = -Wall -g
OUTPUT = index
EXEOBJ = ./indexer.o
HEADERS = ./indexer.h
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(EXEOBJ)
	$(CC) $(CFLAGS) $(EXEOBJ) $(HEADERS) -o $@

clean:
	rm -f *.o $(OUTPUT)
