/*
* Michael Newman && Mark Conley
* Systems Programming
* Assignment 6: malloc() and free()
*/

#include "malloc.h"

#define VERBOSE 1

// may need to not be static - doesn't change
static BlockMetaPtr blockFront;
static BlockMetaPtr blockRear;

static char memblock[MEMSIZE];

MallocMetaPtr mallocMeta;

/* 
* malloc function that uses a char array
* takes in size, returns pointer to the allocated block or (void*)0 on error
*/
void * mymalloc( unsigned int size, char* file, unsigned int line ) {
	if ( (size + METASIZE) > MEMSIZE ) {
		printf("%s", KRED);
		printf("error: not enough space available to fulfill request\n");
		printf("%s", KGRN);
		printf("malloc on line %d in file: \"%s\"\n", line, file);
		printf("%s", KNRM);
		return (void *) 0;
	}

	if ( size == 0 ) {
		printf("%s", KRED);
		printf("error: cannot malloc space of size 0\n");
		printf("%s", KGRN);
		printf("malloc on line %d in file: \"%s\"\n", line, file);
		printf("%s", KNRM);
		return (void *) 0;
	}

	static int initialized = 0;

	BlockMetaPtr current, newBlock;

	/* initialize the block of memory if its not alread initialized */
	if( !initialized ) {
		blockFront = (BlockMetaPtr) memblock;
		blockRear = blockFront;  /* because will only have one block at start */
		blockFront->prev = 0;
		blockFront->size = MEMSIZE - METASIZE;
		blockFront->isFree = 1;
		blockFront->recognition = RECOGNITION;
		initialized = 1;
	}

	if( size < BIGBLOCKSIZE ) {    /* small block - allocate left to right */
		current = blockFront;
		BlockMetaPtr prev = 0;
		do {
			// BlockMetaPtr succ = (BlockMetaPtr) ((char *)current + METASIZE + current->size);
			// BlockMetaPtr succ = (BlockMetaPtr) (META_TO_DATA(current) + current->size);
			BlockMetaPtr succ = NEXTBLOCK(current);

			if (VERBOSE) {
				printf("%s", KCYN);
				printf("\ncurrent->size: %d\n", current->size);
				printf("current->isFree: %d\n", current->isFree);
				printf("current->prev: %p\n", current->prev);
				printf("current address: %p\n\n", current);

				printf("succ->size: %d\n", succ->size);
				printf("succ->isFree: %d\n", succ->isFree);
				printf("succ->prev: %p\n", succ->prev);
				printf("succ address: %p\n", succ);
				int greater = 0;
				if((char*)succ >= (&memblock[0] + MEMSIZE)) greater = 1;
				printf("is next address greater than array? : %d\n\n", greater);
				printf("%s", KNRM);
			}

			if ( current->size < size ){
				prev = current;
				current = succ;
			}
			else if ( !current->isFree ){
				prev = current;
				current = succ;
			}
			/* found it, perfect size */
			else if ( current->size == size ) {
				current->isFree = 0;
				current->recognition = RECOGNITION;   // helps if this has been overwritten previously by a careless user
				
				// if last block, make blockRear
				if( (char*)succ >= (memblock + MEMSIZE) ) {
					blockRear = current;
				}
				
				return (void*) (META_TO_DATA(current));
			}
			/* found it, chop up */
			else {
				newBlock = (BlockMetaPtr)(META_TO_DATA(current) + size);
				newBlock->prev = current;
				newBlock->size = current->size - METASIZE - size;
				newBlock->isFree = 1;
				newBlock->recognition = RECOGNITION;

				// set next's previous to the newBlock
				((BlockMetaPtr)NEXTBLOCK(current))->prev = newBlock;

				current->size = size;
				current->isFree = 0;
				/* 
				* this is done to help the issue of users writing past the space they allocated
				* and effectively writing over the metadata of the next blocks
				*/
				current->prev = prev;
				current->recognition = RECOGNITION;

				if ( (char*)succ < (memblock + MEMSIZE) ){  // if next is within the array
					succ->prev = newBlock;
				} else {   // means newBlock is the rear
					blockRear = newBlock;
				}

				if (VERBOSE) {
					printf("%s", KCYN);
					printf("\nnewBlock->size: %d\n", newBlock->size);
					printf("newBlock->isFree: %d\n", newBlock->isFree);
					printf("newBlock->prev: %p\n", newBlock->prev);
					printf("newBlock address: %p\n", newBlock);
					printf("%s", KNRM);
				}

				return (void*) (META_TO_DATA(current));
			}

		} while( (char*)current < (memblock + MEMSIZE) );

	} else {    /* big block - allocate right to left */
		
		current = blockRear;
		do {
			BlockMetaPtr succ = NEXTBLOCK(current);

			if ( current->size < (size + METASIZE) ){
				current = current->prev;
			}
			else if ( !current->isFree ){
				current = current->prev;
			}
			/* found it, perfect size */
			else if ( current->size == (size + METASIZE) ) {
				current->isFree = 0;
				current->recognition = RECOGNITION;   // helps if this has been overwritten previously by a careless user
				
				// if last block, make blockRear
				if( (char*)succ >= (memblock + MEMSIZE) ) {
					blockRear = current;
				}

				return (void*) META_TO_DATA(current);
			}
			/* found it, chop up */
			else {
				newBlock = (BlockMetaPtr)((char *) current + current->size - size);
				newBlock->prev = current;
				newBlock->isFree = 0;
				newBlock->size = size;
				newBlock->recognition = RECOGNITION;

				// set next's previous to the newBlock
				((BlockMetaPtr)NEXTBLOCK(current))->prev = newBlock;

				current->size = current->size - METASIZE - size;
				current->isFree = 1;
				current->recognition = RECOGNITION;
				
				if ( (char*)succ < (memblock + MEMSIZE) ){  // if next is within the array
					succ->prev = newBlock;
				} else {   // means newBlock is the rear
					blockRear = newBlock;
				}

				// return (void*) ((char *)newBlock + METASIZE);
				return (void*) META_TO_DATA(newBlock);
			}

		} while( (char*)current >= memblock );
	}

	/* error stuff */
	printf("error: unable to malloc\n");

	return (void *) 0;
}

/*
* calloc function
* calls malloc and then clears out the space allocated
*/
void * mycalloc( unsigned int size, char* file, unsigned int line ) {
	char* allocatedBlock = (char*) mymalloc(size, file, line);
	if ( allocatedBlock != 0 ) {
		memset( allocatedBlock, 0, size );
	}
	return allocatedBlock;
}

/*
* realloc function
* takes in a pointer to memory block and reallocates it if possible
*/
void * myrealloc( void * data, unsigned int newSize, char * file, unsigned int line ) {
	BlockMetaPtr current = (BlockMetaPtr)DATA_TO_META(data);
	int oldSize = current->size;
	char * tmp[oldSize];
	memcpy( tmp, data, oldSize );   // hold old data temporarily

	free(data);
	char* newBlock = (char*) mymalloc(newSize, file, line);
	if( newBlock != 0 ) {
		memcpy( newBlock, tmp, oldSize );
	}

	return (void*)newBlock;
}

/*
* free function
* takes in the pointer to the data to free, returns 1 on success 0 on error
*/
int myfree( void * data, char * file, unsigned int line ) {
	
	// outside of the block of memory, obviously a shit pointer
	if ( (char*)data >= (memblock + MEMSIZE) || DATA_TO_META(data) < memblock ) {
		printf("%s", KRED);
		// technically none of this is on the heap because we use a char[ ] array
		printf("error: cannot free something not on the heap\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);
		printf("%s", KNRM);
		return 0;
	}

	BlockMetaPtr current = (BlockMetaPtr) DATA_TO_META(data);

	// check recognition pattern to see if valid
	if ( current->recognition != RECOGNITION ) {
		printf("%s", KRED);
		printf("error: cannot free a pointer not returned by malloc\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);	
		printf("%s", KNRM);
		return 0;
	}

	// check recognition pattern to see if valid
	if ( current->isFree ) {
		printf("%s", KRED);
		printf("error: cannot free already freed block\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);	
		printf("%s", KNRM);
		return 0;
	}

	current->isFree = 1;

	/* now check right and left for merging adjacent free blocks */
	BlockMetaPtr succ = (BlockMetaPtr) NEXTBLOCK(current);
	if ((char*)succ < (memblock + MEMSIZE)) {   // merge with right if free and exists
		if( (succ->isFree) ) {
			if(VERBOSE)  printf("merging right\nblocksize: %d\n",current->size);
			printf("current blocksize: %d\n", current->size);
			
			BlockMetaPtr succSucc = (BlockMetaPtr)NEXTBLOCK(succ);
			if((char*)succSucc < (memblock + MEMSIZE)) {
				succSucc->prev = current;
			}

			current->size += ((succ->size) + METASIZE);

			if(succ == blockRear) {
				printf("merging right: succ is blockRear\n");
				blockRear = current;
			}

			printf("current blocksize after: %d\n", current->size);
			printf("rearSize = %d\n", blockRear->size);
			printf("frontSize = %d\n", blockFront->size);
			printf("frontNextSize = %d\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);
		}
	} else {   // if succ doesn't exist, current is last block
		blockRear = current;
	}
	if ( (char*)current != memblock && (current->prev)->isFree ) {   // merge with left if free
		printf("\nmerging left:current size: %d\n", current->size);
		printf("prev size before: %d\n", (current->prev)->size);
		
		if((char*)succ < (memblock + MEMSIZE)) {
			succ->prev = current->prev;
		}

		(current->prev)->size += (current->size + METASIZE);
		
		printf("prev size after: %d\n", (current->prev)->size);
		
		if( current == blockRear ) {
			printf("merge left: current is blockRear\n");
			blockRear = current->prev;
		}

		// if(VERBOSE)  printf("merging left\nblocksize: %d\n", (current->prev)->size);

	}

	return 1;
}

/*
* solves the issue of not being able to malloc and then do <char * var> = "string"
* also provides protection for writing beyond the bounds of what was allocated cause people are dumb
*/
void addString( void * ptr, char * word ) {
	
	int size = ((BlockMetaPtr)((char*)ptr - METASIZE))->size;
	if(size > strlen(word))  size = strlen(word) + 1;	// dont copy garbage, add 1 if word length for \0
	int i;
	for(i = 0; i<size-1; i++) {
		if(VERBOSE)  printf("%c\n", word[i]);
		*(((char*)ptr)+i) = word[i];
	}
	*(((char*)ptr)+i) = '\0';
}

int main() {
	malloc(5001);

	printf("METASIZE: %lu\n", METASIZE);
	printf("end of data: %p\n", (memblock + MEMSIZE));

	printf("allocate size 20\n");
	char * test = (char*) malloc(20);
	addString(test, "abcd");
	printf("test: %s\n", test);

	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);
	
	printf("allocate size 3\n");
	char * test2 = (char*) malloc(3);
	addString(test2, "123456789");
	printf("test2: %s\n", test2);

	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);

	BlockMetaPtr tmp = (BlockMetaPtr) (test2 - METASIZE);
	tmp = (BlockMetaPtr) (test + strlen(test));
	tmp = (BlockMetaPtr) (test - METASIZE);

	printf("allocate size 2\n");
	char* test3 = (char *) malloc(2);
	addString(test3, "qw");

	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);


	// printf("test2: %s\n", test2);
	// printf("test2 address before: %p\n", test2);
	// test2 = "123456789763524236743674367342673426734267";
	// printf("test2 address after: %p\n", test2);

	printf("test3: %s\n", test3);
	printf("test2: %s\n\n", test2);

	char* tmp2 = (char*)&memblock[23];
	*tmp2 = 't';
	*(tmp2 + 1) = 'e';
	*(tmp2 + 2) = 's';
	*(tmp2 + 3) = 't';

	int i;
	for (i = METASIZE - 1; i < 19+METASIZE; i++) {
		// printf("%c ", test[i]);
		printf("stuff: %c\n", memblock[i]);
	}

	printf("freeing test2 ...\n");
	free(test2);

	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);

	printf("freeing test+1 ...\n");
	free(test+1);
	printf("freeing test-200 ...\n");
	free(test - 200);
	printf("freeing test ...\n");
	free(test);
	
	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);

	printf("freeing test3 ...\n");
	free(test3);

	printf("\nrearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);

	printf("freeing test3 ...\n");
	free(test3);

	test = (char*)malloc(300);
	printf("rearSize = %d\n", blockRear->size);
	printf("frontSize = %d\n", blockFront->size);
	printf("frontNextSize = %d\n", ((BlockMetaPtr)NEXTBLOCK(blockFront))->size);

	return 0;
}