/*
* Michael Newman && Mark Conley
* Systems Programming
* Assignment 6: malloc() and free()
*/

#include "malloc.h"

// may need to not be static - doesn't change
static BlockMetaPtr blockFront;
static BlockMetaPtr blockRear;

static char memblock[MEMSIZE];
long unsigned int blocksAllocated = 0;
long unsigned int spaceAllocated = 0;

/* 
* malloc function that uses a char array
* takes in size, returns pointer to the allocated block or (void*)0 on error
*/
void * mymalloc( unsigned int size, char* file, unsigned int line ) {
	if ((size + METASIZE) > (MEMSIZE - (spaceAllocated + METASIZE)) && (size + METASIZE) != (MEMSIZE - spaceAllocated)) {
		printf("%s", KRED);
		printf("error: not enough space available to fulfill request\n");
		printf("%s", KGRN);
		printf("malloc on line %d in file: \"%s\"\n", line, file);
		printf("%s", KNRM);
		return (void *) 0;
	}

	if ( size <= 0 ) {
		printf("%s", KRED);
		printf("error: cannot malloc space of size %d\n", size);
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
		atexit( leakCheck );
	}

	if( size < BIGBLOCKSIZE ) {    /* small block - allocate left to right */
		current = blockFront;
		BlockMetaPtr prev = 0;
		do {
			BlockMetaPtr succ = NEXTBLOCK(current);

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

				blocksAllocated++;
				spaceAllocated += current->size + METASIZE;

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

				blocksAllocated++;
				spaceAllocated += current->size + METASIZE;

				return (void*) (META_TO_DATA(current));
			}

		} while( (char*)current < (memblock + MEMSIZE) );

	} else {    /* big block - allocate right to left */
		
		current = blockRear;
		BlockMetaPtr succ = NEXTBLOCK(current);
		do {
			if ( current->size < size ){
				succ = current;
				current = current->prev;
			}
			else if ( !current->isFree ){
				succ = current;
				current = current->prev;
			}
			/* found it, perfect size */
			else if ( current->size == size ) {
				current->isFree = 0;
				current->recognition = RECOGNITION;   // helps if this has been overwritten previously by a careless user
				
				// if last block, make blockRear
				if( (char*)succ >= (memblock + MEMSIZE) ) {
					blockRear = current;
				}

				blocksAllocated++;
				spaceAllocated += current->size + METASIZE;

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

				blocksAllocated++;
				spaceAllocated += newBlock->size + METASIZE;

				return (void*) META_TO_DATA(newBlock);
			}

		} while( (char*)current >= memblock );
	}

	printf("%s", KRED);
	printf("error: cannot malloc\n");
	printf("%s", KGRN);
	printf("malloc on line %d in file: \"%s\"\n", line, file);
	printf("%s", KNRM);

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
	if( data == 0 ) { // null pointer
		printf("%s", KRED);
		// technically none of this is on the heap because we use a char[ ] array
		printf("error: cannot realloc null pointer\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);
		printf("%s", KNRM);
		return 0;
	}
	// outside of the block of memory, obviously a shit pointer
	if ( (char*)data >= (memblock + MEMSIZE) || DATA_TO_META(data) < memblock ) {
		printf("%s", KRED);
		// technically none of this is on the heap because we use a char[ ] array
		printf("error: cannot realloc something not on the heap\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);
		printf("%s", KNRM);
		return 0;
	}

	BlockMetaPtr current = (BlockMetaPtr)DATA_TO_META(data);
	if( current->recognition != RECOGNITION ) {
		printf("%s", KRED);
		printf("error: cannot realloc a pointer not returned from malloc/ free\n");
		printf("%s", KGRN);
		printf("realloc on line %d in file: \"%s\"\n", line, file);
		printf("%s", KNRM);
		return (void*)0;
	}
	if( current->isFree ) {
		printf("%s", KRED);
		printf("error: cannot realloc space already freed\n");
		printf("%s", KGRN);
		printf("realloc on line %d in file: \"%s\"\n", line, file);
		printf("%s", KNRM);
		return (void*)0;
	}
	int copySize;
	 if (current->size > newSize) {
	 	copySize = newSize;
	 } else {
	 	copySize = current->size;
	 }

	char * tmp[copySize];
	memcpy( tmp, data, copySize );   // hold old data temporarily

	free(data);
	char* newBlock = (char*) mymalloc(newSize, file, line);
	if( newBlock != 0 ) {
		memcpy( newBlock, tmp, copySize );
	}

	return (void*)newBlock;
}

/*
* free function
* takes in the pointer to the data to free, returns 1 on success 0 on error
*/
int myfree( void * data, char * file, unsigned int line ) {
	if( data == 0 ) { // null pointer
		printf("%s", KRED);
		// technically none of this is on the heap because we use a char[ ] array
		printf("error: cannot free null pointer\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);
		printf("%s", KNRM);
		return 0;
	}
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
		printf("error: cannot free a pointer not returned by malloc/ free\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);	
		printf("%s", KNRM);
		return 0;
	}

	// if free, cannot make free'er
	if ( current->isFree ) {
		printf("%s", KRED);
		printf("error: cannot free already freed block\n");
		printf("%s", KGRN);
		printf("free on line %d in file %s\n", line, file);	
		printf("%s", KNRM);
		return 0;
	}

	blocksAllocated--;
	spaceAllocated -= (current->size + METASIZE);
	current->isFree = 1;

	/* now check right and left for merging adjacent free blocks */
	BlockMetaPtr succ = (BlockMetaPtr) NEXTBLOCK(current);
	if (succ <= blockRear) {   // merge with right if free and exists
		if ((succ->isFree)) {
			
			BlockMetaPtr succSucc = (BlockMetaPtr)NEXTBLOCK(succ);
			if (succSucc <= blockRear) {
				succSucc->prev = current;
			}

			current->size += ((succ->size) + METASIZE);

			if(succ == blockRear) {
				blockRear = current;
			}

		}
	} else {   // if succ doesn't exist, current is last block
		blockRear = current;
	}
	if ( current != blockFront && (current->prev)->isFree ) {   // merge with left if free
		if ( current == blockRear ) {
			blockRear = current->prev;
		}
		
		if ( succ <= blockRear ) {
			succ->prev = current->prev;
		}

		(current->prev)->size += (current->size + METASIZE);
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
		*(((char*)ptr)+i) = word[i];
	}
	*(((char*)ptr)+i) = '\0';
}

/*
* Leak checking
*/
void leakCheck( ) {
	// eek its a leak
	if( blocksAllocated > 0 ) {
		printf("%s", KRED);
		printf("warning: memory leak (did not free all allocated blocks)\n");
		printf("%s", KGRN);
		printf("%lu bytes in %lu blocks\n", spaceAllocated, blocksAllocated);	
		printf("%s", KNRM);
	}
	return;
}