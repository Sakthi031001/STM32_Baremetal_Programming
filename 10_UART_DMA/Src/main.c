#include "stm32f446xx.h"
#include "uart_dma.h"
#include <string.h>
#include <stdio.h>

extern uint8_t g_rx_complete;
extern uint8_t g_uart_complete;
extern uint8_t g_tx_cmplt;

extern char uart_data_buffer[UART_DATA_BUFFER_SIZE];

char msg_buff[120] = {'\0'};

int main()
{
	uart2_dma_rx_tx_init();
	dma1_init();
	dma1_stream5_uart_rx_config();
	sprintf(msg_buff, "Initialization...complete\n\r");
	dma1_stream6_uart_tx_config((uint32_t)msg_buff,strlen(msg_buff));

	while(!g_tx_cmplt){}

	while(1)
	{
		if(g_rx_complete)
		{
			sprintf(msg_buff, "Message Received : %s \n\r", uart_data_buffer);
			g_rx_complete = 0;
			g_uart_complete = 0;
			dma1_stream6_uart_tx_config((uint32_t)msg_buff,strlen(msg_buff));

			while(!g_tx_cmplt){}
			
		}
	}
}
