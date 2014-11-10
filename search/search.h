/*
 * search.h
 * Mark Conley && Michael Newman
 * 
 */

#ifndef SEARCH_H
#define SEARCH_H

#include "uthash.h"

typedef struct FileNode* FileNodePtr;
struct FileNode
{
	char *filename;
	FileNodePtr next;
};

typedef struct Token* TokenPtr;
struct Token
{
	char *key;
	FileNodePtr fileHead;
	UT_hash_handle hh;         /* makes this structure hashable */
};

typedef struct FileInfo* FileInfoPtr;
struct FileInfo
{
	char * key;
	int count;
	UT_hash_handle hh;
};

/* utils */
char * substring(const char *, int, int);
void printFilesFromWord(char *);
int hashFilesFromWord(char *);
void parseFile(FILE *);

/* search functions */
void searchAND(char *);
void searchOR(char *);

 #endif
