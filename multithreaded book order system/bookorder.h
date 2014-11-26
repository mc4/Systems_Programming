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
typedef struct GoodOrders * goodOrdersPtr;
struct GoodOrders {
	char * bookTitle;
	float balance; 
	goodOrdersPtr tail;
}; 

/* circular linked list */
typedef struct BadOrders * badOrdersPtr;
struct BadOrders {
	char * bookTitle;
	float bookPrice;
	badOrdersPtr tail;
};

typedef struct BookOrder * BookOrderPtr;
struct BookOrder {
	char * bookTitle;
	int customerID;
	char * cateogry;
	float bookPrice;
};

typedef struct Customer * CustomerPtr;
struct Customer {
 	int ID;			   /* key for hashing */
 	goodOrdersPtr goodOrders;
 	badOrdersPtr badOrders;
 	char * name;
 	float balance;
 	UT_hash_handle hh;         /* makes this structure hashable */
 	pthread_mutex_t mutex;
};

typedef struct Category * CategoryPtr;
struct Category {
	char * name;		        /* key */
	QueuePtr queue;
	UT_hash_handle hh;         /* makes this structure hashable */
};