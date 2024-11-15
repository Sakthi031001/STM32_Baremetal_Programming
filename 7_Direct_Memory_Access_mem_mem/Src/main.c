#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"


#define BUFFER_SIZE 5

uint16_t sensor_data_arr[BUFFER_SIZE] = {1, 2, 3, 4, 5};
uint16_t temp_data_arr[BUFFER_SIZE];

volatile uint8_t g_transfer_complete;

int main()
{

	g_transfer_complete=0;

	uart2_tx_init();
	dma2_mem_mem_config();
	dma_transfer_start((uint32_t)sensor_data_arr, (uint32_t)temp_data_arr, BUFFER_SIZE);

	// Wait until transfer complete
	while(!g_transfer_complete){}

	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		printf("Temporary buffer [%d]: %d \r\n",i, temp_data_arr[i]);
	}

	g_transfer_complete=0;

	while(1)
	{

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

	// Check if transfer error occurred
	if((DMA2->LISR) & LISR_TEIF0)
	{
		DMA2->LIFCR |= LIFCR_CTEIF0;
	}

}
