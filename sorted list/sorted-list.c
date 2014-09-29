/*
 * sorted-list.c
 */

#include <stdlib.h>
#include "sorted-list.h"

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
	SortedListPtr list = (SortedListPtr)malloc(sizeof(SortedListPtr));
	//list->front = NULL;
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
void SLDestroy(SortedListPtr list);


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

int SLInsert(SortedListPtr list, void *newObj)
{
	//1. if list or newO is null
	if(list == NULL || newObj == NULL){
		return 0; 
	}
	//2. initialize node to be inserted
	NodePtr temp = (NodePtr)malloc(sizeof(NodePtr));
	temp->data = newObj;

	//3. insert at beginning if list is empty
	if(list->size == 0){
		list->front = temp;
		list->size++;
		return 1;
	}
	
	//NodePtr prevptr = NULL;
	NodePtr ptr = list->front;
	NodePtr nextptr = ptr->next;

	//traverses the list to find relevant ptr
	while(ptr != NULL){
		// compare will be -1 when ptr is smaller, 0 when equal, and 1 when newObj is smaller
		int compare = list->compareFunc(ptr, newObj);
		int compareNext = list->compareFunc(ptr->next, newObj);

		//4. duplicate insertion is an error
		if(compare == 0){
			//display error message?
			return 0;
		}

	//5. insert at the end of the list
		//TODO: finish this
		if(compare == 1 && ptr->next == NULL){
			ptr->next = temp;
			list->size++;
			return 1;
		}

	//6. insert in the middle
		if(compare == 1 && compareNext == -1){
			//TODO: check this
			temp->next = ptr->next;
			ptr->next = temp;
			list->size++;
			return 1;
		}

		//7. update ptr
		//prevptr = ptr;
		ptr = ptr->next;
		nextptr = ptr->next; 

	}
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

int SLRemove(SortedListPtr list, void *newObj);


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

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter);


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter );

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

void * SLNextItem(SortedListIteratorPtr iter);