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
#define OPT 1
#define NO_OPT 0
#define ADDNEW 1
#define NONEW 0
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

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

/* deletes and frees the things hashed into fileList */
void resetFileList() {

	FileInfoPtr current, tmp;

	HASH_ITER(hh, fileList, current, tmp) {
		HASH_DEL(fileList, current);
		free(current);
	}
}

/* deletes and frees the things hashed into fileList */
void deleteWordList() {

	TokenPtr current, tmp;

	HASH_ITER(hh, wordList, current, tmp) {
		
		HASH_DEL(wordList, current);

		FileNodePtr tmpF;
		tmpF = current->fileHead;
		while( tmpF != NULL ) {
			current->fileHead = tmpF->next;
			free(tmpF->filename);
			free(tmpF);
			tmpF = current->fileHead;
		}
		free(current->key);
		free(current);
	}
}

/* 
 * prints the results of the search that are stored in the fileList hash table
 * takes in a word count to check for searchAnd, ignores word counts if -1
 */
void printSearchResults( int wordCount ) {
	// if wordCount is -1 ignore counts
	FileInfoPtr current, tmp;
	int count = 0;

	printf("\n");
	HASH_ITER(hh, fileList, current, tmp) {
		if( wordCount == -1 ) {
			printf("%s\n", current->key);
			count++;
		} else if( wordCount == current->count ) {
			printf("%s\n", current->key);
			count++;
		}
	}
	if(count == 0){
		printf("\nYour search produced no results\n");
	}
}


void printAllCrap() {
	printf("\nprint all the things\n");
	FileInfoPtr current, tmp;

	HASH_ITER(hh, fileList, current, tmp) {
		printf("filename: %s\ncount: %d\n", current->key, current->count);
	}
	printf("\n");
}


/* logical and search */
void searchAND(char * input){
	int wordCount = 0;

	// First check if only one word, if so call printFilesFromWord()
	char *tok = strtok(input, DELIM);
	// printf("tok: %s\n", tok);
	
	if(input == NULL) {
		printf("\nInvalid search terms\n");
	}
	
	char *tok1 = tok;
	tok = strtok(NULL, DELIM);
	// printf("tok1 (should be same as tok): %s\n", tok1);
	// printf("new tok: %s\n", tok);
	
	// only one search term inputted
	if(tok == NULL) {
		printFilesFromWord(tok1);
		return;
	}

	// hash on the word in tok1, if returns 0 mean doesn't exist and search and already fails
	if( !hashFilesFromWord(tok1, ADDNEW, NO_OPT, wordCount) ) {
		printf("\nYour search produced no results\n");
		return;
	}
	wordCount++;

	while( tok != NULL ){
		// if the word cannot be hashed because its not found there are no results
		if( !hashFilesFromWord(tok, NONEW, OPT, wordCount) ) {
			printf("\nYour search produced no results\n");
			resetFileList();
			return;	
		}
		wordCount++;

		tok = strtok(NULL, DELIM);
	}

	// printAllCrap();
	// printf("wordCount: %d\n", wordCount);

	/*  ITERATE THROUGH HASHTABLE AND PRINT IF CORRECT COUNTS */
	printSearchResults(wordCount);

	/* FREE ITEMS IN SEARCHING HASH TABLE */
	resetFileList();
}

/* logical or search*/
void searchOR(char * input){
	printf("searchOR\n");

	printf("input is: %s\n", input);
	// printf("tok is: %s\n", tok);

}

/* print  */
void printFilesFromWord(char * word){

	TokenPtr search;
	HASH_FIND_STR(wordList, word, search);

	if( search == NULL ) {
		printf("\n\"%s\" not found\n", word);
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
 *  Hashes the files from the word into fileList variable, also takes in flags for 
 *  returns 0 if the word has no file occurances (ie is not in wordList), 1 if we sucessfully hash files from word
 */
int hashFilesFromWord(char * word, int addNew, int optimize, int wordCount){
	
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
		if( fileSearch == NULL && addNew ) {
			FileInfoPtr newFileInfo = malloc(sizeof(struct FileInfo));
			newFileInfo->key = tmp->filename;
			newFileInfo->count = 1;
			HASH_ADD_STR(fileList, key, newFileInfo);
		} else if( fileSearch != NULL ) {  // if found
			
			// if optimizing check count and delete if count is less than wordCount
			if( optimize && ((fileSearch->count) < wordCount) ) {
				HASH_DEL( fileList, fileSearch );
				free( fileSearch );
			} else {
				fileSearch->count++;
			}
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
 		printf("\ninvalid number of arguments.\n");
 		exit(1);
 	}

 	char * input, *list;
	char * indexedFile; 
	// char * command;
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

	input = (char *)malloc( MAXINPUTLENGTH );
	list = (char *)malloc( MAXINPUTLENGTH );

	while( 1 ){

		printf("\n%sEnter search command%s\n", KCYN, KNRM);
		fflush(stdin);
		fgets(input, MAXINPUTLENGTH, stdin);

		/* Remove trailing newline, if there. */
	    	if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n')){
	        		input[strlen(input) - 1] = '\0';
    		}

		// printf("in main input is: %s\n", input);

		strcpy(list, input+3);
		tok = strtok(input, DELIM);
			
		if(strcmp(tok, "q") == 0){
			printf("quitting\n");
			break;
		} else if(strcmp(tok, "sa") == 0){
			searchAND(list);
			// printFilesFromWord("abc");
		} else if(strcmp(tok, "so") == 0){
			searchOR(list);
			// tok = NULL;
			// break;
		} else {
			printf("Unknown command\n");
		}

		// tok = strtok(NULL, DELIM);
	}

	free(input);
	free(list);
	deleteWordList();

	// testing shit out
	// printShit();

 	return 0;
 }
