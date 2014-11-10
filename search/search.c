/*
 * Mark Conley && Michael Newman
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "search.h"

#define MAXLINELENGTH 1024

TokenPtr wordList = NULL;
FileInfoPtr fileList = NULL;

/* parses file */
void parseFile( FILE * fp ){

	char *word, *tok;
	char line[MAXLINELENGTH];
	int tik = 1; //flag to check file vs filecount
	const int wordIndex = strlen("<list> ");
	const char delim = ' ';
	TokenPtr token;

	while( fgets( line, sizeof(line), fp) ){

		if(strstr(line,"<list>")){

			word = substring( line, wordIndex, strlen(line)-wordIndex );

			if( (token = (TokenPtr) malloc(sizeof(struct Token))) == NULL ) exit(-1);

			token->key = word;
			token->fileHead = NULL;
			HASH_ADD_STR( wordList, key, word );

		} else if(strstr(line,"</list>")){
			token = NULL;
		} else { //file names and counts
			tok = strtok(line, delim);

			while( tok != NULL ){
				//if its a filename tik is true
				if( tik ){

					FileNodePtr file = (FileNodePtr)malloc(sizeof(char) * strlen(tok));
					if(!file){
						printf("malloc error. now exiting.\n");
						exit(1);
					}

					//creating file node to add to list
					char *newFile = (char *)malloc(sizeof(char) * strlen(filename));
					strcpy(newFile, tok);
					file->filename = newFile;
					file->next = NULL;

					//adds filenode to head of linked list
					LL_PREPEND(token->fileHead, file);

					tik = 0;
				} else { //else tik is file count					
					tik = 1;
				}

				tok = strtok(NULL, delim);

			}
		}

	}

}

char * substring( const char * word, int firstIndex, int length ){
	char * s = (char *)malloc(sizeof(char) * (length + 1));
	strncpy(s, (word + firstIndex), length);
	strcat(s, "\0");
	return s;
}

int main(int argc, char ** argv){

 	if(argc != 2){
 		printf("invalid number of arguments.\n")
 		exit(1);
 	}


	char * indexedFile; 
	FILE * fileptr;

	indexedFile = argv[1];
	fileptr = fopen(argv[1], "r");

	if(!fileptr){ 
		printf("file returned null.\n");
		exit(1);
	}

	parseFile(fileptr);



 	return 0;
 }
