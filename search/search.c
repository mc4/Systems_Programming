/*
 * search.c
 * Mark Conley && Michael Newman
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "utlist.h"

#define MAXLINELENGTH 1024
#define MAXINPUTLENGTH 256
#define DELIM " "

TokenPtr wordList = NULL;
FileInfoPtr fileList = NULL;

/* parses file */
void parseFile( FILE * fp ){

	char *word, *tok;
	char line[MAXLINELENGTH];
	int tik; //flag to check file vs filecount
	const int wordIndex = strlen("<list> ");
	// char delim = ' ';
	TokenPtr token;

	while( fgets(line, sizeof(line), fp) ){

		tik = 1;
		if(strstr(line,"<list>")){

			word = substring( line, wordIndex, strlen(line)-wordIndex-1 );
			if( (token = (TokenPtr) malloc(sizeof(struct Token))) == NULL ) exit(-1);

			token->key = word;
			token->fileHead = NULL;
			HASH_ADD_STR( wordList, key, token );

		} else if(strstr(line,"</list>")){
			token = NULL;
		} else { //file names and counts
			tok = strtok(line, DELIM);
			while( tok != NULL ){
				//if its a filename tik is true
				if( tik ){
					// printf("read file: %s\n", tok);

					FileNodePtr file = (FileNodePtr)malloc(sizeof(char) * strlen(tok));
					if(!file){
						printf("malloc error. now exiting.\n");
						exit(1);
					}

					//creating file node to add to list
					char *newFile = (char *)malloc(sizeof(char) * strlen(tok));
					strcpy(newFile, tok);
					file->filename = newFile;
					file->next = NULL;

					//adds filenode to head of linked list
					LL_PREPEND(token->fileHead, file);

					tik = 0;
				} else { //else tik is file count					
					tik = 1;
				}

				tok = strtok(NULL, DELIM);
			}
		}
	}
}

/* logical and search */
void searchAND(char * input, char * tok){
	printf("searchAND\n");
}

/* logical or search*/
void searchOR(char * input, char *tok){
	printf("searchOR\n");
}

/* print  */
void printFilesFromWord(char * word){

	TokenPtr search;
	HASH_FIND_STR(wordList, word, search);

	if( search == NULL ) {
		printf("\"%s\" not found\n", word);
		return;
	} else {

		FileNodePtr tmp = search->fileHead;
		printf("\n%s", tmp->filename);
		
		while( tmp->next != NULL ) { 
			tmp = tmp->next;
			printf(", %s", tmp->filename);
		}
		printf("\n");
	}
}

/*
 *  Hashes the files from the word into fileList variable
 *  returns 0 if the word has no file occurances (ie is not in wordList), 1 if we sucessfully hash files from word
 */
int hashFilesFromWord(char * word){
	
	TokenPtr search;
	HASH_FIND_STR(wordList, word, search);

	if( search == NULL ) {
		return 0;
	}

	FileNodePtr tmp = search->fileHead;
	FileInfoPtr fileSearch;
	while( tmp != NULL ) { 
		HASH_FIND_STR(fileList, tmp->filename, fileSearch);
		
		// if the file is not found, add it to the table
		if( fileSearch == NULL ) {
			FileInfoPtr newFileInfo = malloc(sizeof(struct FileInfo));
			newFileInfo->key = tmp->filename;
			newFileInfo->count = 1;
			HASH_ADD_STR(fileList, key, newFileInfo);
		} else {  // if found, increase count
			fileSearch->count++;
		}
		tmp = tmp->next;
	}
	
	return 1;
}

char * substring( const char * word, int firstIndex, int length ){
	char * s = (char *)malloc(sizeof(char) * (length + 1));
	strncpy(s, (word + firstIndex), length);
	strcat(s, "\0");
	return s;
}

/*
 *  Function for testing wordList structure - **IGNORE ME**
 */
void printShit() {
	int isFirstIter = 1;
	TokenPtr tmp, currentWord;
	HASH_ITER(hh, wordList, currentWord, tmp) {
		// if(isFirstIter){
		// 	isFirstIter = 0;
		// } else {
		// 	printf("\n");
		// }
		// printf("<list> ");
	 //    printf("%s\n", currentWord->key);
		
		// FileNodePtr tmp = currentWord->fileHead;
		
		// printf("%s", tmp->filename);
		
		// while(tmp->next != NULL) { 
		// 	tmp = tmp->next;
		// 	// printf("file: %s   \t count:  %d\n", tmp->filename, tmp->tokenCount);
		// 	printf(" %s", tmp->filename);
		// }
		// // if(currentWord->fileHead == NULL)
		// printf("\n</list>");
		printf("%s\n", currentWord->key);	
	}
}

int main(int argc, char ** argv){

 	if(argc != 2){
 		printf("invalid number of arguments.\n");
 		exit(1);
 	}

 	char * input;
	char * indexedFile; 
	char * command;
	// char delim = ' ';
	FILE * fileptr;
	char * tok;

	indexedFile = argv[1];
	fileptr = fopen(argv[1], "r");

	if(!fileptr){ 
		printf("file returned null.\n");
		exit(1);
	}

	parseFile(fileptr);

	input = (char *)malloc( MAXLINELENGTH );
	printf("Enter search command\n");
	while( 1 ){
		fgets(input, MAXINPUTLENGTH, stdin);

		/* Remove trailing newline, if there. */
    	if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n')){
        	input[strlen(input) - 1] = '\0';
    	}

		//printf("in main input is: %s\n", input);

		fflush(stdin);

		tok = strtok(input, DELIM);
		while( tok != NULL ){
			
			if(strcmp(tok, "q") == 0){
				printf("quitting\n");
				return 0;
			} else if(strcmp(tok, "sa") == 0){
				searchAND(input, tok);
				printFilesFromWord("abc");
			} else if(strcmp(tok, "so") == 0){
				searchOR(input, tok);
				tok = NULL;
				break;
			} else {
				printf("Unknown command\n");
				//continue;
			}

			tok = strtok(NULL, DELIM);
			printf("Enter search command\n");
		}
	
	}

	// testing shit out
	printShit();

 	return 0;
 }
