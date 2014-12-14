/*
* Michael Newman && Mark Conley
* Systems Programming
* Assignment 6: malloc() and free()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* replace malloc and free */
#define malloc( x )  mymalloc( x, __FILE__, __LINE__ )
#define free( x )  myfree( x, __FILE__, __LINE__ )
#define calloc( x )  mycalloc( x, __FILE__, __LINE__ )
#define realloc( x, y )  myrealloc( x, y, __FILE__, __LINE__ )

/* size macros */
#define METASIZE  sizeof(struct BlockMeta)
#define MALLOCMETASIZE  sizeof(struct MallocMeta)
#define MEMSIZE  5000
#define BIGBLOCKSIZE  250

/* conversion macros */
#define DATA_TO_META( x )  ((char*)x - METASIZE)
#define META_TO_DATA( x )  ((char*)x + METASIZE)
#define NEXTBLOCK( x )  (BlockMetaPtr) (META_TO_DATA(x) + (int)x->size)

/* taken from a fortune cookie */
#define RECOGNITION  2818304339

/* colors */
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"
#define KGRN  "\x1b[32m"
#define KRED  "\x1b[31m"

typedef struct BlockMeta * BlockMetaPtr;
struct BlockMeta {
	BlockMetaPtr prev;
	int isFree;
	unsigned int recognition;
	unsigned int size;
};

/*
* malloc function, takes in size to allocate and the file and size macros
* allocated right to left if it is a big block (as defined by the BIGBLOCKSIZE macro)
* left to right if it is a small block
* returns pointer to allocated memory on success, (void*) 0 on error
*/
void * mymalloc( unsigned int, char*, unsigned int );

/*
* calloc function
* calls malloc and then clears out the space allocated
*/
void * mycalloc( unsigned int, char*, unsigned int );

/*
* realloc function
* takes in a pointer to memory block and reallocates it if possible
* returns a pointer to reallocated memory on success, (void*)0 on error
*/
void * myrealloc( void *, unsigned int, char *, unsigned int );

/*
* free function, takes in the pointer to the data to free
* checks for redundant freeing, freeing an invalid piece of data, and freeing before allocating
* also merges adjacent free blocks
* returns 1 on success, 0 on error
*/
int myfree( void *, char *, unsigned int );

/*
* solves the issue of not being able to malloc and then do <char * var> = "string"
* also provides protection for writing beyond the bounds of what was allocated cause people are dumb
*/
void addString( void *, char * );

/*
* Leak checking
*/
void leakCheck( );