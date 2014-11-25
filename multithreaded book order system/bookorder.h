/*
 * Mark Conley && Michael Newman
 */

#include <stdio.h>
#include <stdlib.h> 
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include "uthash.h"

/* circular linked list */
typedef struct goodOrders * goodOrdersPtr;
struct goodOrders {
	char * bookTitle;
	float balance; 
	goodOrdersPtr tail;
}; 

/* circular linked list */
typedef struct badOrders * badOrdersPtr;
struct badOrders {
	char * bookTitle;
	float bookPrice;
	badOrdersPtr tail;
};

typedef struct BookOrder{
	char * bookTitle;
	int customerID;
	char * cateogry;
	float bookPrice;
}BookOrder;

typedef struct Customer * CustomerPtr;
struct Customer {
 	char * name;
 	float balance;
 	int ID;			   /* key for hashing */
 	UT_hash_handle hh;         /* makes this structure hashable */
 	pthread_mutex_t mutex;
};
