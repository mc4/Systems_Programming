#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct QueueNode * QueueNodePtr;	
struct QueueNode {
	void * data;
	QueueNodePtr next;
};

/* First In First Out */
typedef struct Queue * QueuePtr; 
struct Queue {
	QueueNodePtr head;	// dequeue here
	QueueNodePtr tail;	// enqueue here
	int size;
	pthread_mutex_t mutex;
	pthread_cond_t  dataAvailable;
	// pthread_cond_t  spaceAvailable;
};

QueuePtr Qcreate();
void enqueue(QueuePtr, void *);
void * dequeue(QueuePtr);
void * const Qpeek(QueuePtr);
int Qsize(QueuePtr);
void Qdestroyer(QueuePtr);

QueuePtr Qcreate(){
	QueuePtr q = malloc(sizeof(struct Queue));
	if(!q) exit(1);
	q->head   = NULL;
	q->tail   = NULL;
	q->size   = 0;
	pthread_mutex_init(&q->mutex,0);
	return q;
}

void enqueue(QueuePtr q, void * data){
	// pthread_mutex_lock(&q->mutex);
	QueueNodePtr queueNode = malloc(sizeof(struct QueueNode));
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
	// pthread_mutex_unlock(&q->mutex);
}

/* returns the data from the  */
void * dequeue(QueuePtr q){
	// pthread_mutex_lock(&q->mutex);

	if(q->size == 0){
		// pthread_mutex_unlock(&q->mutex);
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
  	// pthread_mutex_unlock(&q->mutex);
	return data;
}

/* looks at the head of the queue without changing it. 
 * returns NULL if queue is empty;
 */
void * const Qpeek(QueuePtr q){
	if(Qsize(q) == 0){
		return NULL;
	}

	void * const value = &q->head;// = malloc(sizeof(struct QueueNode));
	// value = q->head;

	return value;
}

int Qsize(QueuePtr q){
	return q->size;
}

/* this Q is still usable after calling destroyer */
/* dequeue frees all the nodes */
/* NOTE: queue can still be used after this. make sure to no */
void Qdestroyer(QueuePtr q){
	while(dequeue(q) != NULL);
	pthread_mutex_destroy(&q->mutex);
	free(q);
}

// int main(void){
// 	int temp = 0;
// 	int x = 0, y = 0, z = 0;

// 	QueuePtr q = Qcreate();

// 	//enqueue(q, (void*)-1);
// 	enqueue(q, (void*)1);
// 	enqueue(q, (void*)2);
// 	enqueue(q, (void*)3);


// 	// x = (int)Qpeek(q);
// 	dequeue(q);
// 	printf("data is %d, %d, %d\n",x,y,z);

// 	// y = (int)Qpeek(q);
// 	printf("data is %d, %d, %d\n",x,y,z);
// 	dequeue(q);

// 	//z = (int)Qpeek(q);
// 	//printf("data is %d, %d, %d\n",x,y,z);

// 	// int i = (int)dequeue(q);
// 	// int j = (int)dequeue(q);
// 	// int k = (int)dequeue(q);

// 	//printf("data is %d, %d, %d\n",i,j,k);

// 	Qdestroyer(q); 
// 	return 0;
// }
