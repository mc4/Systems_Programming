# Michael Newman && Mark Conley
# makefile for bookorder
# makes an executable bookorder

CC = gcc
CFLAGS = -Wall -g -pthread
BOOKORDEROUTPUT = bookorder
BOOKORDEROBJ = ./bookorder.o
BOOKORDERHEADERS = ./bookorder.h

default: $(BOOKORDEROUTPUT)
all: default 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOKORDEROUTPUT): $(BOOKORDEROBJ)
	$(CC) $(CFLAGS) $(BOOKORDEROBJ) $(BOOKORDERHEADERS) -o $@

clean:
	rm -f *.o $(BOOKORDEROUTPUT)
