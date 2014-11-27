/*
 * Mark Conley && Michael Newman
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> 
#include <limits.h>
#include <pthread.h>
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
	UT_hash_handle hh;         /* makes this structure hashable */
};