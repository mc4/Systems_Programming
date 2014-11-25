/*
 * Mark Conley && Michael Newman
 */

#include "bookorder.h"

#define MAXLINELENGTH 1024

/* hashtable global */
CustomerPtr customers = NULL;
// categories hash table

char * substring( const char * word, int firstIndex, int length ){
	char * s = (char *)malloc(sizeof(char) * (length + 1));
	strncpy(s, (word + firstIndex), length);
	strcat(s, "\0");
	return s;
}

/*
 * Creates a hash table for the customers given a customer database file
 */
void createCustomerDatabase(char * customerDatabase){

	FILE * fp;
	char line[MAXLINELENGTH];
	char * token;
	char * reduce;

	fp = fopen(customerDatabase,"r");
	if(!fp){
		printf("file pointer error on %d. could not open file%s.\n", __LINE__, customerDatabase);
		exit(1);
	}


	while(fgets(line, sizeof(line), fp)){
		CustomerPtr c = (CustomerPtr)malloc(sizeof(struct Customer));

		/* first token is name */
		token = strtok(line, "|");
		reduce = substring(token, 1, (strlen(token) - 2));
		c->name = reduce;
		reduce = NULL;

		/* second token is customer ID */
		token = strtok(NULL, "|");
		int customerID = atoi(token);  //converts string to an integeger
		c->ID = customerID;

		/* third token is the balance */
		token = strtok(NULL, "|");
		float b = atof(token);
		c->balance = b;

		/* remaining tokens consist is the address */

		pthread_mutex_init( &c->mutex, 0 );
		HASH_ADD_INT(customers, ID, c);
		continue;
	}

}

/* 
 * prints the results of the search that are stored in the fileList hash table
 * takes in a word count to check for searchAnd, ignores word counts if -1
 */
void printSearchResults() {
	CustomerPtr current, tmp;
	HASH_ITER(hh, customers, current, tmp) {
		printf("%s | %d | %f \n", current->name, current->ID, current->balance );
	}
}

/*
 * Creates and initializes queues for threads, spawns the threads (consumers),
 * and hashes the queue into the Qtable hash table
 */
void createCategoryThreads( char * categories ) {

}

int main(int argc, char ** argv){

 	if(argc != 4){
 		printf("incorrect number of arguments\n");
 	}

 	char * database   = argv[1];
	char * bookOrders = argv[2];
	char * categories = argv[3];

	createCustomerDatabase(database);
	printSearchResults();

	createCategoryThreads(categories);

	return 0;
}
