
/* Malloc & Free  				 *
 * Test case 3   				 *
 * Theme: demonstrate leak check *
 *        and addString function */

#include "malloc.c"

int main(void){

printf("####################################\n");
printf("we implemented a function that ")
printf("provides protection for writing beyond the bounds of what was allocated.\n\n");
printf("will allocate for 10 bytes of memory and try to write a large string to that memory.\n");
printf("####################################\n\n");

printf("trying to add \"This is a test string.\" to a dynamically allocated char pointer.\n");
char * test = (char *)malloc(10);
addString(test, "This is a test string.");
printf("printing the string that was larger than the space allocated for it\n");
printf("%s\n",test);

printf("Now exiting without freeing to demonstrate leak checker\n");

return 0;
}
