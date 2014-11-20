#include <stdlib.h>
#include <pthread.h>
//#include "queue.h"

typedef struct QueueNode * QueueNodePtr;	
struct QueueNode {
	void *data;
	QueueNodePtr next;
};

typedef struct Queue Queue; 
struct Queue {
	int isOpen;			//flag for when the producer is done
	QueueNodePtr *head;	//dequeue here
	QueueNodePtr *tail;	//enqueue here
	int length;
	pthread_mutex_t mutex;
	pthread_cond_t dataAvailable;
	pthread_cond_t spaceAvailable;
};

Queue *Qcreate();
void enqueue(Queue *, void *);
void *dequeue(Queue *);
int Qlength(Queue *);
void Qdestroyer(Queue *);

Queue * Qcreate(){
	Queue * q = malloc(sizeof(Queue));
	if(!q) exit(1);
	q->isOpen = 1;
	q->head = NULL;
	q->tail = NULL;
	q->length = 0;
	pthread_mutex_init(&q->mutex,0);
	return q;
}

void enqueue(Queue *q, void *data){
	pthread_mutex_lock(&q->mutex);
	QueueNodePtr queueNode = malloc(sizeof(queueNode));
	queueNode->data = data;
	queueNode->next = NULL;
	if(q->length == 0){
		QueueNodePtr queueNode = malloc(sizeof(queueNode));
		queueNode->data = data;
		queueNode->next = NULL;
		q->head = &queueNode;
		q->tail = &queueNode;
	} else {

	}
	


	q->length++;
	pthread_mutex_unlock(&q->mutex);
}

void * dequeue(Queue *q){
	pthread_mutex_lock(&q->mutex);


	q->length--;
  	pthread_mutex_unlock(&q->mutex);
	return data;
}

int Qlength(Queue *q){
	return q->length;
}

void Qdestroyer(Queue *q){
	while(q->length > 0){
		dequeue(q);
	}
	pthread_mutex_destroy(&q->mutex);
}

int main(void){
	return 0;
}
