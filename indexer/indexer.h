#ifndef INDEXER_H
#define INDEXER_H

#include <dirent.h>
#include "uthash.h"

#define BUFFER_LEN 25
#define REALLOC_OFFSET 10
#define KNRM  "\x1B[0m"

typedef struct FileNode* FileNodePtr;
struct FileNode
{
	char *filename;
	int tokenCount;
	FileNodePtr next;
};

typedef struct Token* TokenPtr;
struct Token
{
	char *key;
	FileNodePtr fileHead;
	UT_hash_handle hh;         /* makes this structure hashable */
};

#define KRED  "\x1B[31m"

/*
 * indexer.h 
 * Mark Conley && Michael Newman
 */

/*
 * recursively traverses directory
 * @param directory to traverse
 */
void recursiveDirTraverser( char * );

/*
 * checks whether a directory it is empty
 * @param directory to check whether its empty
 */
int isDirectoryEmpty( char * );

/*
 * Takes in the file name (with path as well) to open
 * calls the readFile function with the open file to read through the file and tokenize the words
 */
void indexFile( char *filename );

/*
 * Adds a file node to the head of the file list in the word given as an argument
 * also sets the count to 1 for that file
 */
void addFileNode( TokenPtr word, char *filename );

/* 
 * indexes a word, calling other functions to check and add to hashtable
 * returns 1 on success, otherwise it returns 0
 */
int indexWord( char *key, char *filename );

#endif