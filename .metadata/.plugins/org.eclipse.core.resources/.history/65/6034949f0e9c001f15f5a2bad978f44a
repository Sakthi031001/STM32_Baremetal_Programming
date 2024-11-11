#include "stm32f446xx.h"
#include <string.h>

char a[1] = {'A'};
char *pt_a = a;

char hello[] ={"Hello! World..."};
char *pt_hello = hello;

int main(void)
{
	for(int i=0; i<strlen(hello); i++)
	{
		ITM_SendChar(*pt_hello++);
	}
	while(1)
	{
		//ITM_SendChar(*pt_a);
	}
}
