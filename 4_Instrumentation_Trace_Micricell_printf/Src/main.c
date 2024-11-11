#include "stm32f446xx.h"
#include <string.h>
#include <stdio.h>

char a[1] = {'A'};
char *pt_a = a;

char hello[] ={"Hello! World..."};
char *pt_hello = hello;

int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

int main(void)
{
	printf("Available \n");

	while(1)
	{

	}
}
