#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct QueueNode * QueueNodePtr;	
struct QueueNode {
	void * data;
	QueueNodePtr next;
};

/* First In First Out */
typedef struct Queue Queue; 
struct Queue {
	int isOpen;			// flag for when the producer is done
	QueueNodePtr head;	// dequeue here
	QueueNodePtr tail;	// enqueue here
	int size;
	pthread_mutex_t mutex;
};

Queue * Qcreate();
void enqueue(Queue *, void *);
void * dequeue(Queue *);
void * Qpeek(Queue *);
int Qsize(Queue *);
void Qdestroyer(Queue *);

Queue * Qcreate(){
	Queue * q = malloc(sizeof(Queue));
	if(!q) exit(1);
	q->isOpen = 1;
	q->head   = NULL;
	q->tail   = NULL;
	q->size = 0;
	pthread_mutex_init(&q->mutex,0);
	return q;
}

void enqueue(Queue * q, void * data){
	pthread_mutex_lock(&q->mutex);
	QueueNodePtr queueNode = malloc(sizeof(queueNode));
	queueNode->data = data;
	queueNode->next = NULL;
	if(q->size == 0){
		queueNode->next = NULL;
		q->head = queueNode;
		q->tail = queueNode;
	} else {
		q->tail->next = queueNode;
		q->tail = queueNode;
	}
	q->size++;
	pthread_mutex_unlock(&q->mutex);
}

void * dequeue(Queue * q){
	pthread_mutex_lock(&q->mutex);

	if(q->size == 0){
		pthread_mutex_unlock(&q->mutex);
		return NULL;
	}

	QueueNodePtr head = q->head;
	void * data = head->data;
	if(q->size == 1){		
		free(head);
	} else {
		q->head = q->head->next;
		free(head);

	}

	q->size--;
  	pthread_mutex_unlock(&q->mutex);
	return data;
}

/* looks at the  of the queue without changing it */
/* TODO: implement this if necessary */
void * Qpeek(Queue * q){
	return NULL;
}

int Qsize(Queue * q){
	return q->size;
}

/* dequeue frees nodes */
void Qdestroyer(Queue * q){
	while(q->size > 0){
		dequeue(q);
	}
	pthread_mutex_destroy(&q->mutex);
}

int main(void){
	Queue * q = Qcreate();

	enqueue(q, (void*)-1);
	enqueue(q, (void*)-2223);
	enqueue(q, (void*)3333);
	printf("q size is: %d = %d\n",q->size, Qsize(q));

	int i = (int)dequeue(q);
	int j = (int)dequeue(q);
	int k = (int)dequeue(q);

	printf("data is %d, %d, %d\n",i,j,k);

	Qdestroyer(q);
	return 0;
}
