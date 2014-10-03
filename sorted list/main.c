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

int main()
{

	SortedListPtr slpInt = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	SortedListPtr slpChar = SLCreate(compareStrings, destroyBasicTypeNoAlloc);
	SortedListPtr slpDouble = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);

	int x, *p, *v;
	p = malloc(sizeof(int));
	// SortedListPtr sl = SLCreate(compareInts);
	// SortedListIteratorPtr si;
    
	while(scanf("%d",&x)==1) {
		v = malloc(sizeof(int));
		*v = x;
		printf("address: %d\n", *(int*)&v);
		SLInsert(slpInt,v);
	}

	char s1[] = "test";
	char s2[] = "zoro";
	SLInsert(slpChar, (void*)&s1);
	SLInsert(slpChar, (void*)&s2);
	SLInsert(slpChar, (void*)"qwerty");
	SLInsert(slpChar, (void*)"alphabet");
	SLInsert(slpChar, (void*)"peanut butter");

	double dub = 1.2;
	double dub2 = 22.1234234234;
	double dub3 = 0.1;
	double dub4 = 12;
	SLInsert(slpDouble, (void*)&dub);
	SLInsert(slpDouble, (void*)&dub2);
	SLInsert(slpDouble, (void*)&dub3);
	SLInsert(slpDouble, (void*)&dub4);

	// // printf("%d\n", strcmp("zoro", "qwerty"));

	printIntList(slpInt);
	printCharList(slpChar);
	printIDoubleList(slpDouble);

	printf("-------------------- iterators --------------------\n");
	SLCreateIterator(slpInt);
	

	SLDestroy(slpInt);
	SLDestroy(slpChar);
	SLDestroy(slpDouble);

	// testHelper(compareInts, destroyBasicTypeNoAlloc);
}