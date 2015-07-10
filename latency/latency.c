#include <stdio.h>
//#define array_size 10
#define TYPE void*

int array_subscript = 0;
TYPE find_one_noloop_element(TYPE* array, TYPE* array_check, int array_size, int array_check_num)
{
	int k,j;
	for( k = array_subscript; k < array_size; k++ )
	{
		for(j = 0; j < array_check_num; j++)
		{
			if(array[k] == array_check[j])
				break;
			if(j == (array_check_num-1))
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
	int array_size = 10;
	TYPE array[array_size];
	TYPE array_check[array_size];
	int i = 0;
	int j = 0;
	TYPE tmp = 0;
	TYPE tmp_bak;
	int count = 20;

	printf("-----------------------------\n");
	while(count--)
	{
		for(i = 0; i < array_size; i++)
			array[i] = &array[i];
		for (i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array[i]);	
		printf("\n");
	
		for (i = array_size-1; i > 1; i--)
		{
			j = rand()%i;
	//		printf("j = %d\n", j);
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
		}
	
		for (i = 0; i < array_size; i++)
			printf("0x%lx ", (long)array[i]);	
		printf("\n");
		
	
		i = 1;
		tmp = array[0];
		array_check[0] = tmp;
		while(i < array_size) // check no loop in the array[]
		{
			tmp_bak = tmp;
//			tmp = array[tmp];
			tmp = (void*)*(long*)tmp;
			for(j = 0; j < i; j++)
			{
			/* test whether have a round in the pointer chasing array, if it does have, pick up one element which not in array_check[], to exchange which array[tmp_bak] */
				if(array_check[j] == tmp)
				{
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
					exchange_element(array,find_one_noloop_element(array, array_check, array_size, i),tmp_bak);
					tmp = (void*)*(long*)tmp_bak;
				}
					
			}
			array_check[i] = tmp;
			i++;
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

