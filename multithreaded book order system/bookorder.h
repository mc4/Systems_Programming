/*
 * Mark Conley && Michael Newman
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> 
#include <limits.h>
#include <pthread.h>
 #include <unistd.h>
#include "uthash.h"
#include "queue.c"

/* circular linked list */
typedef struct GoodOrders * goodOrdersPtr;
struct GoodOrders {
	char * bookTitle;
	float balance; 
	float bookPrice;
	goodOrdersPtr next;
}; 

/* circular linked list */
typedef struct BadOrders * badOrdersPtr;
struct BadOrders {
	char * bookTitle;
	float bookPrice;
	badOrdersPtr next;
};

typedef struct BookOrder * BookOrderPtr;
struct BookOrder {
	char * bookTitle;
	int customerID;
	char * category;
	float bookPrice;
};

typedef struct Customer * CustomerPtr;
struct Customer {
 	int ID;			   /* key for hashing */
 	goodOrdersPtr goodOrdersTail;
 	badOrdersPtr badOrdersTail;
 	char * name;
 	float balance;
 	UT_hash_handle hh;         /* makes this structure hashable */
 	pthread_mutex_t mutex;
};

typedef struct Category * CategoryPtr;
struct Category {
	char * name;		        /* key */
	QueuePtr queue;
	pthread_t tid;		        /* stores the thread id */
	int isOpen;
	int isProcessing;
	UT_hash_handle hh;         /* makes this structure hashable */
};

/*
 * substring function, takes in a pointer to the character array,
 * the index to start at, and the length of the new char array
 */
char * substring( const char * word, int firstIndex, int length );

/*
 * Creates a hash table for the customers given a customer database file
 */
void createCustomerDatabase(char * customerDatabase);

/* 
 * prints the results of the search that are stored in the fileList hash table
 * takes in a word count to check for searchAnd, ignores word counts if -1
 */
void printSearchResults();

/*
 * Consumer thread
 * processes the orders for the given category in the arg
 * arg contains name of category and the queue
 */
 void * consumer( void * );

 /*
  * Producer thread
  * reads the book orders file and processes the orders,
  * placing them into queues based on the category
  */
  void * producer( void * );

/*
 * Creates and initializes queues for threads, spawns the threads (consumers),
 * and hashes the queue into the Qtable hash table
 */
void createCategoryThreads( char * );

/* prints the final report to a file named "finalreport.txt" */
void printFinalReport();