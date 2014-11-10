/*
 * Mark Conley && Michael Newman
 */

#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include "indexer.h"
#include "utlist.h"

TokenPtr words = NULL;

int isDirectoryEmpty( char *dirname ) {
	int n = 0;
	struct dirent *d;
	DIR *dir = opendir(dirname);

	if (dir == NULL){ //Not a directory or doesn't exist
		return 1;
	}

	while ((d = readdir(dir)) != NULL) {
		if(++n > 2){
			break;
		}
	}
	closedir(dir);

	if (n <= 2){ //Directory Empty
		return 1;
	} else {
		return 0;
	}
}

void recursiveDirTraverser( char *entry ){
	
	DIR *pDir;
	struct dirent *pDirent;

	pDir = opendir(entry);
	if(pDir == NULL){
		printf("%sinvalid directory or file:%s %s\n", KRED, KNRM, entry);
		exit(0);
	}
	if((pDirent = readdir(pDir)) == NULL ){
		return;
	}
	if( pDirent->d_type == 8 ){
		char path[PATH_MAX];
		if ((getcwd(path, PATH_MAX)) == NULL) {
	          	perror("pwd");
			exit(1);
		}

		strcat(path, "/");
		strcat(path, pDirent->d_name);
        		
		indexFile ( path );
	}

	
	chdir(entry);

	while( pDirent ){
		if(strcmp(pDirent->d_name, ".")  == 0 || strcmp(pDirent->d_name, "..") == 0 ) {
			pDirent = readdir(pDir);
		          continue;
		 }

		/* d_type == 8 when text file,
		 * if(S_ISREG(statbuf.st_mode)) 
		 */
		if( pDirent->d_type == 8 ){

			char path[PATH_MAX];
			if ((getcwd(path, PATH_MAX)) == NULL) {
		          	perror("pwd");
				exit(1);
			}
			strcat(path, "/");
			strcat(path, pDirent->d_name);
	        		
	        		// pDirent->d_name,
			indexFile ( path );

		/* d_type == 4 when dir
		 * if(S_ISDIR(statbuf.st_mode)) 
		 */
		} else if( pDirent->d_type == 4 ) {

			// if dir has no file
			if(!isDirectoryEmpty(pDirent->d_name)){

				char path[PATH_MAX];
				if ((getcwd(path, PATH_MAX)) == NULL) {
			          	perror("pwd");
					exit(1);
				}
				
				recursiveDirTraverser( pDirent->d_name ); 
				chdir(path);
			}

			chdir(entry); 
		} else {
			/* shouldn't happen */
			printf("errer: pDirent->d_type: %d.\n", pDirent->d_type);
		}

		//redirectd pDir to each file or dir	
		if((pDirent = readdir(pDir)) == NULL ){
			closedir(pDir);
			return;
		}
	}
	closedir(pDir);
	return;
}

/*
 * Reads a file from a file pointer and tokenizes words
 * sends tokenized words off to be added to the hashtable
 */
void readFile ( FILE *file, char* filename ) 
{
	if (file == NULL)  return;
	// add 1 to the max for the null terminating character
	// char word[BUFFER_LEN + 1];
	char *word = (char *) malloc(sizeof(char) * (BUFFER_LEN + 1));

	int c;
	int charCount = 0;
	do {
		c = fgetc(file);

		if (isalnum(c)) {
			if( charCount == BUFFER_LEN ) {
				word = (char *) realloc( word, (BUFFER_LEN + REALLOC_OFFSET) );
			}

			word[charCount] = tolower( c );
			charCount++;
		} else {
			if ( charCount > 0 ) {
				word[ charCount ] = '\0';
				charCount = 0;

				// printf("%s\n", word);
				// send word to function to add to indexed
				indexWord( word, filename );

			}

			if ( feof(file) ) {
				break;
			} else if( charCount > 0 ) {
				word = (char *) malloc(sizeof(char) * (BUFFER_LEN + 1));
			}
		}

	} while(1);
}

/*
 * Takes in the file name (with path as well) to open
 * calls the readFile function with the open file to read through the file and tokenize the words
 */
void indexFile( char *filename )
{
	if ( filename == NULL )  return;

	FILE *fpData;

	// open file given from filename given
	fpData = fopen(filename, "r");

	// check if files exist, if not error message and abort
	if( fpData == NULL ){
		fprintf(stderr, "%sError Opening Files: %s%s%s\n", KRED, KNRM, filename, KRED);
		printf("%s", KNRM);
		return;
	}

	readFile(fpData, filename);

	fclose(fpData);
	return;
}

/*
 * Adds a file node to the head of the file list in the word given as an argument
 * also sets the count to 1 for that file
 */
void addFileNode( TokenPtr word, char *filename ) {
	FileNodePtr fileNode;
	if ( (fileNode = (FileNodePtr) malloc(sizeof(struct FileNode))) == NULL ) exit(-1);
	char *newFilename = (char *) malloc(sizeof(char) * strlen(filename));
	strcpy(newFilename, filename);
	fileNode->filename = newFilename;
	fileNode->tokenCount = 1;
	fileNode->next = NULL;

	// add node to head of list
	LL_PREPEND(word->fileHead, fileNode);
}

/* 
 * indexes a word, calling other functions to check and add to hashtable
 * returns 1 on success, otherwise it returns 0
 */
int indexWord( char *key, char *filename ) {
	if( key == NULL || filename == NULL ) {  // ya dun gooffed
		return 0;
	}

	TokenPtr word, search;

	HASH_FIND_STR(words, key, search);
	// if the word exists in the hash table, check its fileNodes
	if ( search ) {
		// shouldn't happen in current implementation
		if ( search -> fileHead == NULL ) {
			addFileNode( search, filename );
		} else if ( !strcmp(filename, (search->fileHead->filename)) ) {      // file already exists for word
			search->fileHead->tokenCount++;
		} else { // file doesn't exist for word
			addFileNode( search, filename );
		}

	} else {  // word doesn't exist in the hashtable, create new word and file
		if ( (word = (TokenPtr) malloc(sizeof(struct Token))) == NULL ) exit(-1);
		char *newKey = (char *) malloc(sizeof(char) * strlen(key));
		strcpy(newKey, key);
		word -> key = newKey;
		word -> fileHead = NULL;
		HASH_ADD_STR( words, key, word );
		addFileNode( word, filename );
	}
	return 1;
}

int name_sort(TokenPtr a, TokenPtr b) {
	return strcmp(a->key,b->key);
}

int count_sort(FileNodePtr b, FileNodePtr a) {
	return (a->tokenCount - b->tokenCount);
}

/*
 * prints the indexed words and counts to the given output file
 */
void printIndexToFile(char *outputFile, int upDir) {
	if(upDir) {
		chdir("..");
	}
	FILE *fp;
	fp = fopen( outputFile, "w+" );
	HASH_SORT( words, name_sort );

	int isFirstIter = 1;
	TokenPtr tmp, currentWord;
	HASH_ITER(hh, words, currentWord, tmp) {
		if(isFirstIter){
			isFirstIter = 0;
		} else {
			fprintf(fp, "\n");
		}
		fprintf(fp, "<list> ");
	      	fprintf(fp, "%s\n", currentWord->key);
		
		LL_SORT(currentWord->fileHead, count_sort);
		FileNodePtr tmp = currentWord->fileHead;
		
		fprintf(fp, "%s %d", tmp->filename, tmp->tokenCount);
		
		while(tmp->next != NULL) {
			tmp = tmp->next;
			// printf("file: %s   \t count:  %d\n", tmp->filename, tmp->tokenCount);
			fprintf(fp, " %s %d", tmp->filename, tmp->tokenCount);
		}
		// if(currentWord->fileHead == NULL)
		fprintf(fp, "\n</list>");
	}
	fclose(fp);
}

int isFile( const char *path){
	struct stat buf;
	stat(path, &buf);
	return S_ISREG(buf.st_mode);
}

int main(int argc, char **argv){
 	
 	char *outputFile = argv[1];
 	char *input = argv[2];

	if(argc != 3){
		printf("%sinvalid arguments%s\n", KRED, KNRM);
		exit(1);
	} else if(strcmp(outputFile, "") == 0 || outputFile == NULL) {
		printf("%sinvalid output file%s\n", KRED, KNRM);
		exit(1);
	} else if(strcmp(input, "") == 0 || input == NULL) {
		printf("%sinvalid input file%s\n",KRED, KNRM);
		exit(1);
	}

	int upDir = 1;
	if(isFile( input )){
		indexFile( input );
		upDir = 0;
	} else {
	 	recursiveDirTraverser( input );
	}

 	printIndexToFile( outputFile, upDir );

 	return 0;
}