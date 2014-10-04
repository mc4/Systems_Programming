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

	int choice;
	printf("Please enter a test case number 1 - 8.\n");
	scanf("%d", &choice);

	switch(choice){
		case 1:
			printf("Comparing Ints.\n");
			printf("Input is: 10, 9, 8, 7, 6\n");

			int a = 10;
			int b = 9;
			int c = 8;
			int d = 7;
			int e = 6;

			SortedListPtr slpInt = SLCreate(compareInts, destroyBasicTypeNoAlloc);
			SLInsert(slpInt, (void*)&a);
			SLInsert(slpInt, (void*)&b);
			SLInsert(slpInt, (void*)&c);
			SLInsert(slpInt, (void*)&d);
			SLInsert(slpInt, (void*)&e);

			printf("Expected output: 10, 9, 8, 7, 6\n");
			printIntList(slpInt);
			SLDestroy(slpInt);
			break;
		case 2:
			printf("Comparing Ints.\n");
			printf("Input is: 10, 9, 8, 7, 6\n");

			double f = 22.1;
			double g = 25.;
			double h = 34.3455;
			double i = 3.3;
			double j = 3.14295;

			SortedListPtr slpDouble = SLCreate(compareDoubles, destroyBasicTypeNoAlloc);
			SLInsert(slpDouble, (void*)&f);
			SLInsert(slpDouble, (void*)&g);
			SLInsert(slpDouble, (void*)&h);
			SLInsert(slpDouble, (void*)&i);
			SLInsert(slpDouble, (void*)&j);

			printf("Expected output: 34.3455, 25, 22.1, 3.3, 3.14295\n");
			printDoubleList(slpDouble);
			SLDestroy(slpDouble);
			break;
		case 3:
			printf("Comparing Strings.\n");
			printf("Input is: test, zoro, qwerty, alphabet, peanut butter\n");

			char *k = "test";
			char *l = "zoro";
			char *m = "qwerty";
			char *n = "alphabet";
			char *o = "peanut butter";

			SortedListPtr slp = SLCreate(compareInts, destroyBasicTypeNoAlloc);
			SLInsert(slp, (void*)&k);
			SLInsert(slp, (void*)&l);
			SLInsert(slp, (void*)&m);
			SLInsert(slp, (void*)&n);
			SLInsert(slp, (void*)&o);

			printf("Expected output: zoro, test, qwerty, peanut butter, alphabet\n");
			printf("Result: zoro, test, qwerty, peanut butter, alphabet\n");	
			SLDestroy(slp);
			break;
		case 4:
			printf("Comparing Chars.\n");
			printf("Input is: a, b, z, y, c, d, w, x\n");

			char p = 'a';
			char q = 'b';
			char r = 'z';
			char s = 'y';
			char t = 'c';

			SortedListPtr slpChar = SLCreate(compareInts, destroyBasicTypeNoAlloc);
			SLInsert(slpChar, (void*)&p);
			SLInsert(slpChar, (void*)&q);
			SLInsert(slpChar, (void*)&r);
			SLInsert(slpChar, (void*)&s);
			SLInsert(slpChar, (void*)&t);

			printf("Expected output: z, y, c, b, a\n");
			printCharList(slpChar);
			SLDestroy(slpChar);	
			break;
		case 5:
			printf("Comparing Ints.\n");
			printf("Input is: 10, 9, 8, 7, 6\n");

			int u = 10;
			int v = 9;
			int w = 8;
			int x = 7;
			int y = 6;

			SortedListPtr slpInt2 = SLCreate(compareInts, destroyBasicTypeNoAlloc);
			SLInsert(slpInt2, (void*)&u);
			SLInsert(slpInt2, (void*)&v);
			SLRemove(slpInt2, (void*)&u); //removing10
			SLInsert(slpInt2, (void*)&w);
			SLInsert(slpInt2, (void*)&x);
			SLRemove(slpInt2, (void*)&v); //removing 9
			SLInsert(slpInt2, (void*)&y);

			printf("Expected output: 8, 7, 6\n");
			printIntList(slpInt2);
			SLDestroy(slpInt2);		
			break;
		case 6:
			printf("Compare Ints - remove 9 while iterator is on 9 and remove 6 while iterator is on 6\n");
			printf("Input is: 10, 9, 8, 7, 6\n");

			int aa = 10;
			int bb = 9;
			int cc = 8;
			int dd = 7;
			int ee = 6;

			SortedListPtr slpInt3 = SLCreate(compareInts, destroyBasicTypeNoAlloc);
			int ptr;

			// int* iterData = (int*)SLGetItem(intIter1);
			// printListFromIterator(slpInt);

			SLInsert(slpInt3, (void*)&aa);
			SLInsert(slpInt3, (void*)&bb);
			SLInsert(slpInt3, (void*)&cc);
			SLInsert(slpInt3, (void*)&dd);
			SLInsert(slpInt3, (void*)&ee);

			SortedListIteratorPtr iter = SLCreateIterator(slpInt3);

			ptr = *(int*)SLGetItem(iter); //returns 10
			ptr = *(int*)SLNextItem(iter); //returns 9
			SLRemove(slpInt3, (void*)&bb);
			ptr = *(int*)SLNextItem(iter); //returns 8
			ptr = *(int*)SLNextItem(iter); //returns 7
			ptr = *(int*)SLNextItem(iter); //returns 6
			SLRemove(slpInt3, (void*)&ee);

			printf("Expected output: 10, 8, 7\n");
			printIntList(slpInt3);

			SLDestroy(slpInt3);
			SLDestroyIterator(iter);
			break;
		case 7:
		//remove while iter


		case 8:
		default:
			printf("error, bad input.\n");
	}

	return 0;
}