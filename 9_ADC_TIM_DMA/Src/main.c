#include "stm32f446xx.h"
#include <stdio.h>
#include "adc_dma.h"
#include "uart.h"
#include "dma.h"

extern uint16_t adc_raw_data[NUM_OF_SAMPLES];

volatile uint8_t g_transfer_complete;

int main()
{
	g_transfer_complete = 0;
	uart2_tx_init();
	adc_tim_dma_init();

	while(1)
	{
		if(g_transfer_complete)
		{
			g_transfer_complete = 0;

			for(int i=0; i < NUM_OF_SAMPLES; i++)
			{
				printf("SAMPLE NUMBER [%d] : %d\n\r", i, adc_raw_data[i]);
			}
		}

		for(int i=0; i<90000; i++){}
	}
}

void DMA2_Stream0_IRQHandler(void)
{
	// Check if transfer control interrupt occurred
	if((DMA2->LISR) & LISR_TCIF0)
	{
		g_transfer_complete =  1;

		// Clear flag
		DMA2->LIFCR |= LIFCR_CTCIF0;
	}
}
