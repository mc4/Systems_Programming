/*
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

/* returns a substring from of a word */
char * substring(const char *, int, int);

/* search... */
void parseFile(FILE *);


 #endif
