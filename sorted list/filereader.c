#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
FILE *fp;
int c;
// test
/* The first argument is the program name, The second argument is usually file */
if(argc != 2)   {
	printf("Incorrect number of arguments.\n");
<<<<<<< HEAD
	return 5;
=======
	return 7;
>>>>>>> 44ac602b7b4e4d019a5a6ae09925c67755b0b175
}


fp = fopen(argv[1],"r"); 

if(fp == NULL){
	printf("File returned NULL.\n");
	exit(1);
}


/* Scans document character by character */
while ( (c = fgetc(fp)) != EOF ){
<<<<<<< HEAD

	printf("WE NEEEEEEDDDDDD THISSSSSSS%c\n", c);
=======
>>>>>>> 44ac602b7b4e4d019a5a6ae09925c67755b0b175
	
}

fclose(fp);
return 0; 
}
