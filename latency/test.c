#include <stdio.h>
#include <stdlib.h>

static inline unsigned long long my_rand (unsigned long long limit)
{
	if(limit == 0)
		return 0;
	else
		return ((unsigned long long)(((unsigned long long)rand()<<48)^((unsigned long long)rand()<<32)^((unsigned long long)rand()<<16)^(unsigned long long)rand())) % limit;
}

int main()
{
	int size = 80;
	int last_size =1;
	int cycle_length = 4;
	int page_size = 40;
	void **array = NULL;
	array = new void*[size];
	void *pos = array;

	if (last_size == 1)     //first iteration
	        array[0] = &array[0];
	for (long long i=1;i<size;i++)
	        array[i] = &array[i];
	
	for(long long i = 0; i < size; i++)
		printf("0x%lx ", array[i]);
	printf("\n");
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

	        register void* p = &array[i];
	        while(*(void **)p != (void *)&array[i])
	        {
	                p = *(void **)p;
	        }
	        if(i+page_size >= size)
	               *(void **) p = &array[0];
		else
	        	*(void **)p = &array[i+page_size];
	}

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
	return 0;
}
