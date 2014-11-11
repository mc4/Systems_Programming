# Michael Newman && Mark Conley
# makefile for search
# makes an executable search

CC = gcc
CFLAGS = -Wall -g
SEARCHOUTPUT = search
SEARCHOBJ = ./search.o
SEARCHHEADERS = ./search.h
INDEXEROUTPUT = indexer
INDEXEROBJ = ./indexer.o
INDEXERHEADERS = ./indexer.h

default: $(SEARCHOUTPUT) $(INDEXEROUTPUT)
all: default 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SEARCHOUTPUT): $(SEARCHOBJ)
	$(CC) $(CFLAGS) $(SEARCHOBJ) $(SEARCHHEADERS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INDEXEROUTPUT): $(INDEXEROBJ)
	$(CC) $(CFLAGS) $(INDEXEROBJ) $(INDEXERHEADERS) -o $@

clean:
	rm -f *.o $(SEARCHOUTPUT) $(INDEXEROUTPUT)
