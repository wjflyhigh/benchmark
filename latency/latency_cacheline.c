#include <stdio.h>
//#define array_size 10
#define TYPE void*
int array_subscript;

static inline unsigned long long my_rand (unsigned long long limit)
{
	return ((unsigned long long)(((unsigned long long)rand()<<48)^((unsigned long long)rand()<<32)^((unsigned long long)rand()<<16)^(unsigned long long)rand())) % limit;
}

TYPE find_one_noloop_element(TYPE* array, TYPE* array_check, int cycle_length, int array_size, int array_check_num, int row_num)
{
	int k,j;
	for( k = array_subscript; k < (array_size - cycle_length + row_num); k = k+cycle_length )
	{
		for(j = array_subscript; j < array_check_num; j = j+cycle_length)
		{
			if(array[k] == array_check[j])
				break;
			if(j == (array_check_num-cycle_length))
				return &array[k];
		}
	}
	
}

void exchange_element(TYPE *array, TYPE k, TYPE j)
{
	TYPE tmp = (void*)*(long*)k;
	*(long*)k = (long)*(long*)j;
	*(long*)j = (long)tmp;
}

int main()
{
	int array_size = 20;
//	int cycle_length = 128/8;
	int cycle_length = 2;
	TYPE array[array_size];
	TYPE array_check[array_size];
	int i = 0;
	int j = 0;
	int k = 0; 
	TYPE tmp = NULL;
	TYPE tmp_bak = NULL;
	int count = 20;

	printf("-----------------------------\n");
	while(count--)
	{
		for(i = 0; i < array_size; i++)
			array[i] = &array[i];
		for (i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array[i]);	
		printf("\n");

		for(i = 0; i < array_size; i++)
			array_check[i] = NULL;
	
		for (i = array_size-1; i > cycle_length; i--)
		{
			j = my_rand(i/cycle_length) * cycle_length + (i%cycle_length);
//			j = rand()%(i/cycle_length) * cycle_length + (i%cycle_length);
//			j = rand()%i;
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
		}
	
		for (i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array[i]);	
		printf("\n");
		
	for(k = 0; k < cycle_length; k++)
	{	
		array_subscript = k;
		i = cycle_length + k;
		tmp = array[k];
		array_check[k] = tmp;
		while(i < (array_size-cycle_length+k)) // check no loop in the array[]
		{
			tmp_bak = tmp;
			tmp = (void*)*(long*)tmp;
			for(j = k; j < i; j = (j+cycle_length))
			{
			/* test whether have a round in the pointer chasing array, if it does have, pick up one element which not in array_check[], to exchange which array[tmp_bak] */
				if(array_check[j] == tmp)
				{
					printf("?????????????????????????????????????????\n");
					exchange_element(array,find_one_noloop_element(array, array_check, cycle_length, array_size, i, k),tmp_bak);
					tmp = (void*)*(long*)tmp_bak;
				}
					
			}
			array_check[i] = tmp;
			i = i + cycle_length;
		}
	}
	
		for(i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array[i]);
	
		printf("\n");
	
		for (i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array_check[i]);	
		printf("\n");
		printf("-----------------------------\n");
	}
	return 0;
}

