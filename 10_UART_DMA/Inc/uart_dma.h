#ifndef __UART_DMA_H__
#define __UART_DMA_H__

#include "stm32f446xx.h"
#include <stdint.h>

#define UART_DATA_BUFFER_SIZE   5

void uart2_dma_rx_tx_init(void);
void dma1_init();
void dma1_stream5_uart_rx_config();
void dma1_stream6_uart_tx_config(uint32_t msg_to_send, uint32_t msg_len);



#endif
