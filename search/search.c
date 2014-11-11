/*
 * search.c
 * Mark Conley && Michael Newman
 *
 */

 #include "search.h"

#define MAXINPUTLENGTH 1024
#define DELIM " "
#define OPT 1
#define NO_OPT 0
#define ADDNEW 1
#define NONEW 0
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

/* Vars for hashtables */
TokenPtr wordList = NULL;
FileInfoPtr fileList = NULL;

/* parses file */
void parseFile( FILE * fp ){

	char *word, *tok;
	char line[MAXINPUTLENGTH];
	int tik; //flag to check file vs filecount
	const int wordIndex = strlen("<list> ");
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
		printf("Your search produced no results\n");
	}
}

/* logical and search */
void searchAND(char * input){
	int wordCount = 0;

	// First check if only one word, if so call printFilesFromWord()
	char *tok = strtok(input, DELIM);
	
	if( input == NULL || tok == NULL ) {
		printf("\nInvalid search terms\n");
		return;
	}
	
	char *tok1 = tok;
	tok = strtok(NULL, DELIM);
	
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

	/*  ITERATE THROUGH HASHTABLE AND PRINT IF CORRECT COUNTS */
	printSearchResults(wordCount);

	/* FREE ITEMS IN SEARCHING HASH TABLE */
	resetFileList();
}

/* logical or search*/
void searchOR(char * input){

	// First check if only one word, if so call printFilesFromWord()
	char *tok = strtok(input, DELIM);

	if( input == NULL || tok == NULL ) {
		printf("\nInvalid search terms\n");
		return;
	}
	
	char *tok1 = tok;
	tok = strtok(NULL, DELIM);
	
	// only one search term inputted
	if(tok == NULL) {
		printFilesFromWord(tok1);
		return;
	}

	// hash on the word in tok1
	hashFilesFromWord(tok1, ADDNEW, NO_OPT, 0);

	while( tok != NULL ){
		// if the word cannot be hashed because its not found there are no results
		hashFilesFromWord(tok, ADDNEW, NO_OPT, 0);

		tok = strtok(NULL, DELIM);
	}

	/*  ITERATE THROUGH HASHTABLE AND PRINT IF CORRECT COUNTS (-1 to not check word counts) */
	printSearchResults(-1);

	/* FREE ITEMS IN SEARCHING HASH TABLE */
	resetFileList();

}

/* 
 * takes in a single word and prints all files that word occurs in
 */
void printFilesFromWord(char * word){

	TokenPtr search;
	HASH_FIND_STR(wordList, word, search);
	printf("\n");
	if( search == NULL ) {
		printf("Your search produced no results\n");
		return;
	} else {

		FileNodePtr tmp = search->fileHead;
		printf("%s", tmp->filename);
		
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

int main(int argc, char ** argv){

 	if(argc != 2){
 		printf("\ninvalid number of arguments.\n");
 		exit(1);
 	}

 	char * input, *list;
	char * indexedFile; 
	FILE * fileptr;
	char * tok;

	indexedFile = argv[1];
	fileptr = fopen(indexedFile, "r");

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

		// user pressed enter without entering text
		if(strlen(input) == 1) {
			printf("\nPlease enter a command\n");
			continue;
		}

		/* Remove trailing newline, if there. */
	    	if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n')){
	        		input[strlen(input) - 1] = '\0';
    		}

		strcpy(list, input+3);   // would need to be changed if commands with length > 2
		tok = strtok(input, DELIM);
			
		if(strcmp(tok, "q") == 0){
			printf("\nquitting\n");
			break;
		} else if(strcmp(tok, "sa") == 0){
			searchAND(list);
		} else if(strcmp(tok, "so") == 0){
			searchOR(list);
		} else {
			printf("\nUnknown command\n");
		}

	}

	free(input);
	free(list);
	deleteWordList();

 	return 0;
 }
