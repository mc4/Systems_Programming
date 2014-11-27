/*
 * Mark Conley && Michael Newman
 */

#include "bookorder.h"

#define MAXLINELENGTH 1024

/* hashtable global */
CustomerPtr customers = NULL;
CategoryPtr Qtable = NULL;

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
 * Consumer thread
 * processes the orders for the given category in the arg
 * arg contains name of category and the queue
 */
 void * consumer( void * arg ) {
 	
 	CategoryPtr category = (CategoryPtr) arg;
 	QueuePtr Q = category->queue;
 	char * catName = category->name;
 	/* declare more vars */

 	pthread_detach( pthread_self() );

 	// process while the category is still set to be open
 	while( category->isOpen ) {
 		// lock the queue
 		pthread_mutex_lock( &Q->mutex );
 		while( Q->size == 0 ) {  // wait when there is nothing in the queue to process
 			printf("%s consumer waits because queue is empty.\n", catName);
 			pthread_cond_wait( &Q->dataAvailable, &Q->mutex );
 		}

 		printf("%s consumer resumes because queue has order(s) ready for processing\n", catName);

 		BookOrderPtr order;
 		while( (order = (BookOrderPtr) dequeue(Q)) != NULL ) {   // while there is something in the queue to process
 			CustomerPtr customer;
 			// find customer from order
 			HASH_FIND_INT( customers, &order->customerID,  customer);

 			// lock mutex for customer
 			pthread_mutex_lock( &customer->mutex );

 			// check if price of order greater than balance
 			if( order->bookPrice > customer->balance ) {
 				badOrdersPtr tmpBad = ( badOrdersPtr ) malloc(sizeof(struct BadOrders));
 				tmpBad->bookTitle = order->bookTitle;
 				tmpBad->bookPrice = order->bookPrice;
 				
 				if( customer->badOrdersTail == NULL ) {
 					tmpBad->next = tmpBad;
 					customer->badOrdersTail = tmpBad;
 				} else {
					tmpBad->next = customer->badOrdersTail->next;
					customer->badOrdersTail->next = tmpBad;
					customer->badOrdersTail = tmpBad;
				}
 			} else {
 				goodOrdersPtr tmpGood = ( goodOrdersPtr ) malloc(sizeof(struct GoodOrders));
 				tmpGood->bookTitle = order->bookTitle;
 				tmpGood->bookPrice = order->bookPrice;
 				customer->balance = customer->balance - order->bookPrice;
 				tmpGood->balance = customer->balance;

 				if( customer->goodOrdersTail == NULL ) {
 					tmpGood->next = tmpGood;
 					customer->goodOrdersTail = tmpGood;
 				} else {
 					tmpGood->next = customer->goodOrdersTail->next;
 					customer->goodOrdersTail->next = tmpGood;
 					customer->goodOrdersTail = tmpGood;
 				}
 			}

 			// unlock mutex for customer
 			pthread_mutex_unlock( &customer->mutex );
 		}
 		// pthread_cond_signal( &Q->dataAvailable );		// shout at consumer
 		pthread_mutex_unlock( &Q->mutex );		// unlock mutex for the queue
 	}

 	return 0;
 }

 /*
  * Producer thread
  * reads the book orders file and processes the orders,
  * placing them into queues based on the category
  */
  void * producer( void * arg ) {
  	char * bookOrders = (char *) arg;
	char line[MAXLINELENGTH];
  	char * token;
  	FILE * fp;
  	/* declare more vars */

  	pthread_detach( pthread_self() );


	fp = fopen(bookOrders,"r");
	if(!fp){
		printf("file pointer error on %d. could not open file%s.\n", __LINE__, bookOrders);
		exit(1);
	}

	while(fgets(line, sizeof(line), fp)){
		BookOrderPtr tmpOrder = (BookOrderPtr) malloc(sizeof(struct BookOrder));

		/* first token is book title */
		token = strtok(line, "|");
		tmpOrder->bookTitle = substring(token, 1, (strlen(token) - 2));

		/* second token is price */
		token = strtok(NULL, "|");
		float price = atof(token);
		tmpOrder->bookPrice = price;

		/* third token is the customer ID */
		token = strtok(NULL, "|");
		int customerID = atoi(token);  //converts string to an integeger
		tmpOrder->customerID = customerID;

		/* fourth token is category */
		token = strtok(line, "|");
		tmpOrder->category = substring(token, 0, strlen(token));

		// Find Q in Qtable by category
		CategoryPtr categoryData;
		HASH_FIND_STR(Qtable, tmpOrder->category, categoryData);
		QueuePtr Q = categoryData->queue;

		// Obtain lock for Q
		pthread_mutex_lock(&Q->mutex);  // may need to change this (ie get rid of &)

		// Put order into Q
		enqueue(Q, tmpOrder);

		// Unlock Q
		pthread_mutex_unlock(&Q->mutex);  // may need to change this (ie get rid of &)

// Signal consumer ... change to be more efficient
		pthread_cond_signal( &Q->dataAvailable );
	}

	// close down consumers wait on consumers to finish
	CategoryPtr current, tmp;
	// iterate through the queues and wait the threads attached to each queue to close
	HASH_ITER(hh, Qtable, current, tmp) {
		current->isOpen = 0;
		pthread_join(current->tid, NULL);
	}

	// print overall report ?

	return 0;
  }

/*
 * Creates and initializes queues for threads, spawns the threads (consumers),
 * and hashes the queue into the Qtable hash table
 */
void createCategoryThreads( char * categories ) {
	
	FILE * fp;
	char line[MAXLINELENGTH];

	fp = fopen(categories,"r");
	if(!fp){
		printf("file pointer error on %d. could not open file%s.\n", __LINE__, categories);
		exit(1);
	}


	while(fgets(line, sizeof(line), fp)){
		int lineLen = strlen(line);
		if(lineLen <= 0)  continue;  // if empty line continue
		
		// upper case the characters
		int i = lineLen -1;
		for( ; i >= 0; i-- ) {
			toupper( line[i] );
		}

		// create new queue and init
		QueuePtr q = Qcreate();

		// create struct
		CategoryPtr tmpCat  = (CategoryPtr) malloc(sizeof(struct Category));		
		
		char * name;
		// check if line has a new line or EOF at end, if so trim
		if( line[lineLen-2] == '\n' || line[lineLen-2] == EOF ) {  // shouldn't be EOF, check anyway
			name  = (char *) malloc(sizeof(char) * lineLen -1);
			strncpy(name, line, (lineLen-1));
			name[lineLen-2] = '\0';
		} else {
			name  = (char *) malloc(sizeof(char) * lineLen);
			strncpy(name, line, (lineLen));
			name[lineLen-1] = '\0';  // redundant
		}


		tmpCat->name = name;
		tmpCat->queue = q;
		tmpCat->isOpen = 1;      /* set category to open for processing */

		// hash into Qtable
		HASH_ADD_STR(Qtable, name, tmpCat);

		// spawn consumer
		pthread_create( &tmpCat->tid, 0, consumer, tmpCat );

	}
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

	// make queues and spawn consumers
	createCategoryThreads(categories);

	// spawn producer
	pthread_t ignore;
	pthread_create( &ignore, 0, producer, bookOrders );

	return 0;
}
