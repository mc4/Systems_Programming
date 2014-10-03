/*
 * sorted-list.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"

static void SLDeleteNode(NodePtr, DestructFuncT);
static void printIntList(SortedListPtr);
static void printCharList(SortedListPtr);
static void printIDoubleList(SortedListPtr list);

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.	
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
	if(cf == NULL || df == NULL)  return NULL;

	SortedListPtr list = (SortedListPtr)malloc(sizeof(struct SortedList));
	list->front = NULL;
	list->size  = 0;
	list->compareFunc = cf;
	list->destroyFunc = df;
	return list;
}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list) {
	
	/* anything else to free, besides the node itself and its data? */

	DestructFuncT df = list->destroyFunc;
	while(list->front != NULL) {
		NodePtr tmp = list->front;
		list->front = tmp->next;

		// first free the data inside the node, then the node itself		
		df( tmp->data );
		free( tmp );
	}
} 

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj) {
	// error, bad input
	if(list == NULL || newObj == NULL){
		return 0; 
	}

	NodePtr newNode = (NodePtr)malloc(sizeof(struct Node));
	newNode->data = newObj;
	newNode->next = NULL;
	newNode->refCount = 1;

	// no head, new node is head
	if(list->size == 0) {
		list->front = newNode;
		list->size++;
		return 1;
	}

	NodePtr current = list->front;
	NodePtr prev = NULL;
	
	CompareFuncT cf = list->compareFunc;
	int compare = cf ( (current->data), newObj );

	// duplicate insertion
	if( compare == 0 ) {
		free(newNode);
		return 0;
	}

	// add before head
	if( compare < 0 ) {
		newNode->next = list->front;
		list->front = newNode;
		list->size++;
		return 1;
	}

	while(1) {
		prev = current;
		current = current->next;

		// add to tail
		if(current == NULL) {
			prev->next = newNode;
			list->size++;
			return 1;
		}
		
		compare = cf ( (current->data), newObj );

		// duplicate insertion
		if( compare == 0 ) {
			free(newNode);
			return 0;
		}

		// add to middle, before current
		if( compare < 0 ) {
			newNode->next = current;
			prev->next = newNode;
			list->size++;
			return 1;
		}
	}
	// shouldn't happen
	free(newNode);
	return 0;
}

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj) {
	if ( list == NULL || newObj == NULL ) {
		return 0;
	}

	NodePtr current = list->front;
	NodePtr prev = NULL;

	// compare will be -1 when current is smaller, 0 when equal, and 1 when newObj is smaller
	int compare = list->compareFunc( (current->data), newObj);
	int isHead = ((compare == 0) ? 1 : 0);
	
	// iterate through list until we either find a match or newObj is greater than the current
	while ( compare > 0 ) {
		prev = current;
		current = current->next;
		if ( current == NULL )  break;  // reached end of the list
		compare = list->compareFunc( (current->data), newObj );
	}

	// If we found a matching node remove it from the list (not necessarrily delete it yet)
	if ( compare == 0 ) {
		NodePtr next = current->next;
		// check if it is the head that matches
		if ( isHead ) {
			list->front = next;  // next is initalized to null, so no need to check for that
		} else {
			prev->next = next;
		}

		// if current node not tail, increment the refCount of the next node
		if ( next != NULL ) {
			next->refCount++;
		}
		
		// decrement ref count if greater than 1, otherwise fully delete node
		if ( current->refCount > 1 ) {
			current->refCount--;
		} else {
			SLDeleteNode( current, list->destroyFunc );
		}
		list->size--;
		return 1;
	}
	return 0;
}

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	if(list <= 0) return NULL; 
	SortedListIteratorPtr iter = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	iter->current = list->front;
	iter->current->refCount++;
	iter->destroyFunc = list->destroyFunc;
	return iter;
}

/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter) {
	iter->current->refCount--;
	if ( iter->current->refCount < 1 ) {
		SLDeleteNode( iter->current, iter->destroyFunc );
	}
	free(iter);
}

/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
	NodePtr ptr = (NodePtr)malloc(sizeof(NodePtr));
	ptr = iter->current;
	ptr->data = iter->current->data;
	//ptr->next = NULL;
	//ptr->refCount++;
	return (void *) ptr->data;
}

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter){
	if(iter == NULL || iter->current == NULL || iter->current->next == NULL){ 
		return NULL; 
	}

	NodePtr ptr = (NodePtr)malloc(sizeof(NodePtr)); 
	ptr = iter->current;
	ptr->refCount--;

	if(ptr->refCount == 0){
		NodePtr temp = (NodePtr)malloc(sizeof(NodePtr));
		while(ptr->refCount == 0){
			temp = ptr;
			ptr = ptr->next;
			SLDeleteNode(temp, iter->destroyFunc); //also decrements next ptr's refCount
		}	
	}

	ptr = ptr->next;
	ptr->refCount++;
	iter->current = ptr;

	void* data = iter->current->data;
	return data;
}

/* 
 * Frees space allocated by node, and decrements refCount of next node 
 * Only called when a node no longer has anything pointing to it.
 */

static void SLDeleteNode(NodePtr ptr, DestructFuncT df){
	if(ptr->next != NULL){
		ptr->next->refCount--;
	}
	df( ptr->data );
	free(ptr);
}

static void printIntList(SortedListPtr list){
 	NodePtr ptr = (NodePtr)malloc(sizeof(NodePtr));
 	
	for(ptr = list->front; ptr != NULL; ptr = ptr->next){
		printf("%d->", *(int*)ptr->data);
	}
	printf("\n");
}

static void printIDoubleList(SortedListPtr list){
 	NodePtr ptr = (NodePtr)malloc(sizeof(NodePtr));
 	
	for(ptr = list->front; ptr != NULL; ptr = ptr->next){
		printf("%f->", *(double*)ptr->data);
	}
	printf("\n");
}

static void printCharList(SortedListPtr list){
 	NodePtr ptr = (NodePtr)malloc(sizeof(NodePtr));
	 
	for(ptr = list->front; ptr != NULL; ptr = ptr->next){
		printf("%c->", *(char*)ptr->data);
	}
		printf("\n");
}