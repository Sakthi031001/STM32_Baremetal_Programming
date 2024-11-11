#include "swo.h"
#include "stm32f446xx.h"
#include <stdio.h>

int __io_putchar(int ch)
{
	ITM_SendChar(ch);

	return ch;
}

void log_error(char *p)
{
	printf("log Error: ");
	printf((char *)p);
	printf("\r\n");
}

void log_info(char *p)
{
	printf("Log Info: ");
	printf((char *)p);
	printf("\r\n");
}

void log_debug(char *p)
{
	printf("Log Debug: ");
	printf((char *)p);
	printf("\r\n");
}
