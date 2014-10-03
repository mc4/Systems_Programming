/*
 * sorted-list.c
 */

#include	<string.h>
#include	"sorted-list.c"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p){
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for allocated memory (malloc,calloc,etc.)
	free(p);
}

void destroyBasicTypeNoAlloc(void *p) {
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
	return;
}

void printListFromIterator(SortedListPtr slpInt) {
	// create iterator
	SortedListIteratorPtr intIter = SLCreateIterator(slpInt);
	
	int* iterData = (int*)SLGetItem(intIter);
	printf("%d -> ", *iterData);

	for(iterData = (int*)SLNextItem(intIter);iterData != NULL; iterData = (int*)SLNextItem(intIter)) {
		printf("%d -> ", *iterData);
	}
	printf("\n\n");
	SLDestroyIterator(intIter);
}

int main()
{

	SortedListPtr slpInt = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	//SortedListPtr slpChar = SLCreate(compareStrings, destroyBasicTypeNoAlloc);
	//SortedListPtr slpDouble = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);

	// int x, *p, *v;
	// p = malloc(sizeof(int));
	// // SortedListPtr sl = SLCreate(compareInts);
	// // SortedListIteratorPtr si;
    
	// while(scanf("%d",&x)==1) {
	// 	v = malloc(sizeof(int));
	// 	*v = x;
	// 	printf("address: %d\n", *(int*)&v);
	// 	SLInsert(slpInt,v);
	// }

	//100, 45, 50, 76, 90, 99, 56
	// int a = 100;
	// int b = 45;
	// int c = 50;
	// int d = 76;
	// int e = 90;
	// int f = 99;
	// int g = 56;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;
	int test;

	SLInsert(slpInt, (void*)&a);
	SLInsert(slpInt, (void*)&b);
	SLInsert(slpInt, (void*)&c);
	// test = SLRemove(slpInt, (void*)&a);
	SLInsert(slpInt, (void*)&d);
	SLInsert(slpInt, (void*)&e);
	SLInsert(slpInt, (void*)&f);
	SLInsert(slpInt, (void*)&g);

	printf("%d\n", test);

	// char s1[] = "test";
	// char s2[] = "zoro";
	// SLInsert(slpChar, (void*)&s1);
	// SLInsert(slpChar, (void*)&s2);
	// SLInsert(slpChar, (void*)"qwerty");
	// SLInsert(slpChar, (void*)"alphabet");
	// SLInsert(slpChar, (void*)"peanut butter");

	// double dub = 1.2;
	// double dub2 = 22.1234234234;
	// double dub3 = 0.1;
	// double dub4 = 12;
	// SLInsert(slpDouble, (void*)&dub);
	// SLInsert(slpDouble, (void*)&dub2);
	// SLInsert(slpDouble, (void*)&dub3);
	// SLInsert(slpDouble, (void*)&dub4);

	// printf("%d\n", strcmp("zoro", "qwerty"));

	printIntList(slpInt);
	// printCharList(slpChar);
	// printIDoubleList(slpDouble);

	printf("\n-------------------- iterators --------------------\n\n");
	
	printf("Initial list\n");
	printListFromIterator(slpInt);
	
	// create iterator
	SortedListIteratorPtr intIter1 = SLCreateIterator(slpInt);
	printListFromIterator(slpInt);
	
	int* iterData = (int*)SLGetItem(intIter1);
	printf(" Current ptr data:%d\n", *iterData);
	printListFromIterator(slpInt);

	int x;
	while(scanf("%d",&x)==1) {
		// iterate to next node
		if(x == 1) {
			iterData = (int*)SLNextItem(intIter1);
			if(iterData != NULL) {
				printf("Current ptr data:%d\n", *iterData);
			}
			else printf("eror, end of list\n");
			printListFromIterator(slpInt);
		}

		// delete current node
		else if(x == 2) {
			printf("Removing current ptr data:%d\n", *iterData);
			int remove = SLRemove(slpInt, iterData);
			if(!remove) printf("error: failed to remove\n");
			printListFromIterator(slpInt);	
			printf("current node: %d\n", *(int*)SLGetItem(intIter1));
		}

		else if(x == 3) {
			int* iterData = (int*)SLGetItem(intIter1);
			printf(" Current ptr data:%d\n", *iterData);
			printListFromIterator(slpInt);
		}
	}

	
	printListFromIterator(slpInt);
	


	SLDestroy(slpInt);
	// SLDestroy(slpChar);
	// SLDestroy(slpDouble);

	// testHelper(compareInts, destroyBasicTypeNoAlloc);

return 0;
}

/*
int main()
{

	SortedListPtr slpInt = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	//SortedListPtr slpChar = SLCreate(compareStrings, destroyBasicTypeNoAlloc);
	//SortedListPtr slpDouble = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);

	// int x, *p, *v;
	// p = malloc(sizeof(int));
	// // SortedListPtr sl = SLCreate(compareInts);
	// // SortedListIteratorPtr si;
    
	// while(scanf("%d",&x)==1) {
	// 	v = malloc(sizeof(int));
	// 	*v = x;
	// 	printf("address: %d\n", *(int*)&v);
	// 	SLInsert(slpInt,v);
	// }

	//100, 45, 50, 76, 90, 99, 56
	int *a = 100;
	int *b = 45;
	int *c = 50;
	int *d = 76;
	int *e = 90;
	int *f = 99;
	int *g = 56;
	int *h = 100;

	SLInsert(slpInt, (void*)&a);
	SLInsert(slpInt, (void*)&b);
	SLInsert(slpInt, (void*)&c);
	SLInsert(slpInt, (void*)&d);
	SLInsert(slpInt, (void*)&e);
	//SLInsert(slpInt, (void*)&f);
	//SLInsert(slpInt, (void*)&g);

	// char s1[] = "test";
	// char s2[] = "zoro";
	// SLInsert(slpChar, (void*)&s1);
	// SLInsert(slpChar, (void*)&s2);
	// SLInsert(slpChar, (void*)"qwerty");
	// SLInsert(slpChar, (void*)"alphabet");
	// SLInsert(slpChar, (void*)"peanut butter");

	// double dub = 1.2;
	// double dub2 = 22.1234234234;
	// double dub3 = 0.1;
	// double dub4 = 12;
	// SLInsert(slpDouble, (void*)&dub);
	// SLInsert(slpDouble, (void*)&dub2);
	// SLInsert(slpDouble, (void*)&dub3);
	// SLInsert(slpDouble, (void*)&dub4);

	// printf("%d\n", strcmp("zoro", "qwerty"));

	printIntList(slpInt);
	// printCharList(slpChar);
	// printIDoubleList(slpDouble);
	
	printf("-------------------- iterators --------------------\n");
	
	// create iterator
	SortedListIteratorPtr intIter = SLCreateIterator(slpInt);
	intIter->compareFunc = compareInts;
	CompareFuncT cf = intIter->compareFunc;
	int compare;

	compare = cf( (void*)&a, (void*)&d);
	printf("compare test: %d\n", compare);	
	
	int* iterData = (int*)SLGetItem(intIter);
	printf("iterData: %d\n", *iterData);

	//ONLY WORKS WHEN LIST SIZE EQUALS 5
	int i;
	for(i = 0; i < 4; i++){
	iterData = (int*)SLNextItem(intIter);
		printf("iterData: %d\n", *iterData);

	}

	// for(iterData = (int*)SLNextItem(intIter);
	// 	iterData != NULL; 
	// 	iterData = (int*)SLNextItem(intIter)) {
	// 	printf("%d\n", *iterData);
	// }

	printf("reached end of list\n");

	SLDestroy(slpInt);
	// SLDestroy(slpChar);
	// SLDestroy(slpDouble);

	// testHelper(compareInts, destroyBasicTypeNoAlloc);

return 0;
}
*/