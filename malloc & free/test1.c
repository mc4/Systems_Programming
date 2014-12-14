/* Malloc & Free  				 *
 * Test case 1   				 *
 * Theme: high saturation and    *
 *         fragmentation         */

/****************************************************
 * This test will intentionally create a scenario   *
 * where the "heap" is highly fragmented. This will *
 * be done with a calculated series of mallocs and  *
 * frees.  											*
 ****************************************************/

#include "malloc.c"

int main(void){ 											

printf("#######################################################\n");
printf("This test will intentionally create a scenario ");   
printf("where the "heap" is highly fragmented. This will "); 
printf("be done with a calculated series of mallocs and frees.");
printf("refer to the source code in test1.c\n")
printf("#######################################################\n");

/* malloc small chunks, anything less than 250 
   is not considered a big block. small blocks 
   are malloced from left to the right */
char * a = (char *)malloc(200);
char * b = (char *)malloc(1);
char * c = (char *)malloc(249);
char * d = (char *)malloc(50);
char * e = (char *)calloc(249); //calloc
char * f = (char *)malloc(3);
char * g = (char *)malloc(100);
char * h = (char *)malloc(1);
char * i = (char *)malloc(249);
char * j = (char *)calloc(50); //calloc
char * k = (char *)malloc(249);
char * l = (char *)malloc(1);

/* malloc big chunks, anything greater than 250 
   is considered a big block. big blocks 
   are malloced from righter to the left */
char * j = (char *)malloc(1000);
char * k = (char *)malloc(250);
char * l = (char *)calloc(500); //calloc
char * m = (char *)malloc(250);
char * n = (char *)calloc(700); //calloc
char * o = (char *)malloc(450);

//~4,450 allocated. ~4,882 blocks including meta data

/* free specific blocks to cause fragmentation */
free(b);
free(d);
free(f);
free(h);
free(j);
free(l);

free(k);
free(m);

/* now our "heap" is in a very fragmented state */

char * p = (char *)malloc(50);
char * q = (char *)malloc(50);
char * r = (char *)malloc(200);
char * s = (char *)calloc(500); //calloc

j = realloc(j, 500);
l = realloc(l, 400);

/* free everything */
free(a);
free(c);
free(e);
free(g);
free(i);
free(k);

free(j);
free(l);
free(n);
free(o);
free(p);
free(q);
free(r);
free(s);

return 0;
}
