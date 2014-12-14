# Michael Newman && Mark Conley
# makefile for malloc & free
# Makes an executable called 

CC = gcc
CFLAGS = -Wall -g
OUTPUT = ./malloc
EXEOBJ = ./malloc.o
HEADERS = ./malloc.h

TEST1OUTPUT = test1
TEST1ROBJ = ./test1.o
TEST2OUTPUT = test2
TEST2OBJ = ./test2.o
TEST3OUTPUT = test3
TEST3OBJ = ./test3.o

default: test2
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test1: $(EXEOBJ) $(TEST1OBJ)
 	$(CC) $(CFLAGS) $(TEST1OBJ) $(EXEOBJ) $(HEADERS) -o $@

test2: $(EXEOBJ) $(TEST2OBJ)
 	$(CC) $(CFLAGS) $(TEST2OBJ) $(EXEOBJ) $(HEADERS) -o $@

test3: $(EXEOBJ) $(TEST3OBJ)
 	$(CC) $(CFLAGS) $(TEST3OBJ) $(EXEOBJ) $(HEADERS) -o $@

clean:
	rm -f *.o $(TEST1OUTPUT) $(TEST2OUTPUT) $(TEST3OUTPUT)
