#include "stm32f446xx.h"
#include <stdio.h>
#include "adc_dma.h"
#include "uart.h"
#include "dma.h"

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];

int main()
{
	uart2_tx_init();
	adc_dma_init();

	while(1)
	{
		printf("value from sensor 1: %d\n\r", adc_raw_data[0]);
		printf("value from sensor 2: %d\n\r", adc_raw_data[1]);

		for(int i=0; i<90000; i++){}
	}
}
