#include <stdio.h>
#include <stdlib.h>

#define SPEED 8388608
void * dummy;

static inline unsigned long long my_rand (unsigned long long limit)
{
	if(limit == 0)
		return 0;
	else
		return ((unsigned long long)(((unsigned long long)rand()<<48)^((unsigned long long)rand()<<32)^((unsigned long long)rand()<<16)^(unsigned long long)rand())) % limit;
}

inline unsigned long long int rdtsc()
{
	unsigned int lo, hi;

	__asm__ volatile (".byte 0x0f, 0x31" : "=a" (lo), "=d" (hi));
	return (long long)(((unsigned long long)hi)<<32LL) | (unsigned long long) lo;
}

static double run_randlatinpage(void* pos, int ITS, long size)
{
	double clocks_per_it;
	register void* j = pos;

	long long start = rdtsc();	
	for (int i=ITS;i;i--)
	{
/*		if(j == pos)
		{
			printf("=============%ld\n", i);
		}
*/		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
		j = *(void**)j;
	}	
	long long stop = rdtsc();

	clocks_per_it =  (double)(stop-start)/(ITS*16);
	printf ("%lld\t%f\n", size*sizeof(void*), clocks_per_it, dummy);	//passing dummy to prevent optimization
	return clocks_per_it;
}
int main()
{
	//long size = 12;
	long size = 1024*131072;
	int last_size =1;
	int cycle_length = 8;
	int page_size = 512;
	void **array = NULL;
	array = new void*[size];
	void *pos = array;
	int its = 250229;

	if (last_size == 1)     //first iteration
	        array[0] = &array[0];
	for (long long i=1;i<size;i++)
	        array[i] = &array[i];
/*	
	for(long long i = 0; i < size; i++)
	{
		if(i%cycle_length == 0)
		{
			printf("\n");
		}
		if(i%page_size == 0)
			printf("--------------------------------------------\n");
		printf("0x%lx ", array[i]);
	}
	printf("\n");
*/
	//Use a variation on Sattolo's algorithm to incrementally generate a random cyclic permutation that increases in size each time.
	for (long long i=0;i<size;i=i+page_size)
	{
	        for (long long j=page_size-1;j>0;j--)
	        {
	                unsigned int k = my_rand(j/cycle_length) * cycle_length + (j%cycle_length);
	                void* temp = array[j+i];
	                array[j+i] = array[k+i];
	                array[k+i] = temp;
	        }

		for(long long q = 0; q < cycle_length; q++)
		{
	        	register void* p = &array[i+q];
			while(*(void **)p != (void *)&array[i+q])
	        	{
	               		p = *(void **)p;
			}
			if(i+q+page_size >= size)
				*(void **)p = &array[q];
			else
	        		*(void **)p = &array[i+q+page_size];
		}
	}
/*
	for(long long i = 0; i < size; i++)
	{
		if(i%cycle_length == 0)
		{
			printf("\n");
		}
		if(i%page_size == 0)
			printf("--------------------------------------------\n");
		printf("0x%lx ", array[i]);
	}
	printf("\n");
*/
	run_randlatinpage(array, its, size);
	delete[] array;
	return 0;
}
