#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
FILE *fp;
int c;

/* The first argument is the program name, The second argument is usually file */
if(argc != 2)   {
	printf("Incorrect number of arguments.\n");
	return -1;
}


fp = fopen(argv[1],"r"); 

if(fp == NULL){
	printf("File returned NULL.\n");
	exit(1);
}


/* Scans document character by character */
while ( (c = fgetc(fp)) != EOF ){

	printf("%c\n", c);
	
}

fclose(fp);
return 0; 
}