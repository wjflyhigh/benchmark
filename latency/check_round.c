#include <stdio.h>
//#define array_size 10
#define TYPE int

int array_subscript = 0;
int find_one_noloop_element(TYPE* array, TYPE* array_check, int array_size, int array_check_num)
{
	int k,j;
	for( k = array_subscript; k < array_size; k++ )
	{
		for(j = 0; j < array_check_num; j++)
		{
			if(array[k] == array_check[j])
				break;
			if(j == (array_check_num-1))
				return k;
		}
	}
	
}

void exchange_element(TYPE *array, int k, int j)
{
	TYPE tmp = array[k];
	array[k] = array[j];
	array[j] = tmp;
}

int main()
{
	int array_size = 10;
	TYPE array[array_size];
	TYPE array_check[array_size];
	int i = 0;
	int j = 0;
	int tmp = 0;
	int count = 20;
	int tmp_bak;

	printf("-----------------------------\n");
	while(count--)
	{
		for(i = 0; i < array_size; i++)
			array[i] = i;
	
		for (i = array_size-1; i > 1; i--)
		{
			j = rand()%i;
	//		printf("j = %d\n", j);
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
		}
	
		for (i = 0; i < array_size; i++)
			printf("%d ", array[i]);	
		printf("\n");
		
	
		i = 1;
		tmp = array[0];
		array_check[0] = tmp;
		while(i < array_size) // check no loop in the array[]
		{
			tmp_bak = tmp;
			tmp = array[tmp];
			for(j = 0; j < i; j++)
			{
			/* test whether have a round in the pointer chasing array, if it does have, pick up one element which not in array_check[], to exchange which array[tmp_bak] */
				if(array_check[j] == tmp)
				{
					exchange_element(array,find_one_noloop_element(array, array_check, array_size, i),tmp_bak);
					tmp = array[tmp_bak];
				}
					
			}
			array_check[i] = tmp;
			i++;
		}
		
	
		for(i = 0; i < array_size; i++)
			printf("%d ", array[i]);
	
		printf("\n");
	
		for (i = 0; i < array_size; i++)
			printf("%d ", array_check[i]);	
		printf("\n");
		printf("-----------------------------\n");
	}
	return 0;
}

