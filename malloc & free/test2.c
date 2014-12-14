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
/* should throw error since bigInt was never allocated */
printf("####################################\n");
printf("Tring to free an int that was never dynamically allocated\n");
printf("####################################\n");
free(bigInt)

int * testInt = (int *)malloc(10);
/* should throw errors because these address were never returned by malloc */
printf("####################################\n");
printf("Tring to free a dynamically allocated int pointer ");
printf("at an address not returned by malloc\n");
printf("####################################\n");
free(testInt + 1);
free(testInt + 100);
free(testInt - 100);

/* free correctly */
free(testInt);

/* should throw error since it's redundant freeing */
printf("#################\n");
printf("Redundant freeing\n");
printf("#################\n");
free(testInt);

char * pointer = "why is this due at 9 a.m.???"
/* should throw error because pointer was never allocated */
printf("#################################################################\n");
printf("Tring to free a char pointer that was never dynamically allocated\n");
printf("#################################################################\n");
free(pointer);

/* reusing a variable that has already malloc'd and free'd */
printf("############################################################\n");
printf("reusing a variable that has already been malloc'd and free'd\n");
printf("############################################################\n");
int * testInt = (int *)calloc(100);
testInt = realloc(testInt, 100);
free(testInt);

/* should throw error since our "heap" is only a 5000 char array */
printf("######################################################\n");
printf("Attempting to allocate space greater than maximum size\n");
printf("######################################################\n");
char * tooBig = (char *)malloc(5001);

/* should throw error because pointer was never malloc'd */
printf("#######################################################\n");
printf("Tring to realloc a char pointer that was never malloc'd\n");
printf("#######################################################\n");
char * thing = realloc(thing, 100);

printf("#######################################\n");
printf("callocing for the maximum size possible\n");
printf("#######################################\n");
char * wow = (char *)calloc(4876);

/* shouldn't work */
printf("###################################################\n");
printf("Tring to malloc after "heap" is at maximum capacity\n");
printf("###################################################\n");
char * stuff = (char *)malloc(1);

free(wow);

return 0;
}
