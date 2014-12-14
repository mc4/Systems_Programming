/* Malloc & Free  				 *
 * Test case 2   				 *
 * Theme: can we break it?       */

/****************************************************
 * This test will intentionally create a scenario   *
 * where I try to catch errors in malloc and free	*
 ****************************************************/

#include "malloc.c"

int main(void){

int bigInt = 2,100,000;
/* should throw error since bigInt was never allocated*/
free(bigInt)

int * testInt = (int *)malloc(10);
/* should throw errors because these address were never returned by malloc */
free(testInt + 1);
free(testInt + 100);
free(testInt - 100);

/* free correctly */
free(testInt);

/* should throw error since it's redundant freeing */
free(testInt);

char * pointer = "why is this due at 9 a.m.???"
/* should throw error because pointer was never allocated */
free(pointer);

/* reusing a variable that has already malloc'd and free'd */
int * testInt = (int *)calloc(100);
testInt = realloc(testInt, 100);
free(testInt);

/* should throw error since our "heap" is only a 5000 char array */
char * tooBig = (char *)malloc(5001);

/* should throw error because pointer was never malloc'd */
char * thing = realloc(thing, 100);

char * wow = (char *)calloc(4876);

/* shouldn't work */
char * stuff = (char *)malloc(1);

free(wow);
return 0;
}
