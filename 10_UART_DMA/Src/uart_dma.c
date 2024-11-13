#include "uart_dma.h"

#define UART2EN			(1U<<17)
#define GPIOAEN			(1U<<0)

#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)

#define CR3DMAT			(1U<<7)
#define CR3DMAR			(1U<<6)
#define SR_TC			(1U<<6)
#define CR1_TCIE		(1U<<6)

#define DMA1EN			(1U<<21)

#define UART_BAUDRATE	115200
#define CLK				16000000

#define DMA_SCR_EN		(1U<<0)
#define DMA_SCR_MINC	(1U<<10)
#define DMA_SCR_PINC	(1U<<9)
#define DMA_SCR_CIRC	(1U<<8)
#define DMA_SCR_TCIE	(1U<<4)
#define DMA_SCR_TEIE	(1U<<2)
#define DMA_SFCR_DMDIS	(1U<<2)

#define HIFCR_CDMEIF5	(1U<<8)
#define HIFCR_CTEIF5	(1U<<9)
#define HIFCR_CTCIF5	(1U<<11)

#define HIFCR_CDMEIF6	(1U<<18)
#define HIFCR_CTEIF6	(1U<<19)
#define HIFCR_CTCIF6	(1U<<21)

#define HIFSR_TCIF5	(1U<<11)
#define HIFSR_TCIF6	(1U<<21)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
//static void uart2_write(int ch);

char uart_data_buffer[UART_DATA_BUFFER_SIZE];

uint8_t g_rx_complete;
uint8_t g_tx_cmplt;

uint8_t g_uart_complete;

void uart2_dma_rx_tx_init(void)
{
	/************Configure UART GPIO pin************/
	// 1. Enable clock access
	RCC->AHB1ENR |= GPIOAEN;

	// 2. Set PA2 mode to Alternate Function mode
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	// 3. Set PA3 mode to Alternate Function mode
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |=  (1U<<7);

	// 4. Set PA2 Alternate Function type to AF7 (UART_TX)
	GPIOA->AFR[0] |=  (1U<<8) | (1U<<9) | (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	// 5. Set PA3 Alternate Function type to AF7 (UART_TX)
	GPIOA->AFR[0] |=  (1U<<12) | (1U<<13) | (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);


	/************CONFIGURE UART MODULE************/
	// 6. ENABLE CLOCK ACCESS TO UART2
	RCC->APB1ENR |= UART2EN;

	// 7. Set Baudrate
	uart_set_baudrate(CLK, UART_BAUDRATE);

	// 8. Select to use DMA
	USART2->CR3 |= CR3DMAT | CR3DMAR;

	// 9. Set transfer direction
	USART2->CR1 = CR1_TE | CR1_RE;

	// 10. Clear TC flag
	USART2->SR &= ~SR_TC;

	// 11. Enable TCIE
	USART2->CR1 |= CR1_TCIE;

	// 12. Enable UART module
	USART2->CR1 |= CR1_UE;
}

void dma1_init()
{
	// Enable clock access to dma1
	RCC->AHB1ENR |= DMA1EN;

	// Enable DMA Stream6 Interrupting NVIC
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void dma1_stream5_uart_rx_config()
{
	// Disable DMA Stream
	DMA1_Stream5->CR &= ~DMA_SCR_EN;

	// Wait till DMA is disabled
	while((DMA1_Stream5->CR & DMA_SCR_EN)){}

	// Clear interrupt flags for Stream5
	DMA1->HIFCR |= HIFCR_CDMEIF5 | HIFCR_CTEIF5 | HIFCR_CTCIF5;

	// Set peripheral address
	DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

	// Set memory address
	DMA1_Stream5->M0AR = (uint32_t)(&uart_data_buffer);

	// Set number of transfer
	DMA1_Stream5->NDTR  = (uint16_t)(UART_DATA_BUFFER_SIZE);

	// Select channel 4
	DMA1_Stream5->CR |= (1U<<27);
	DMA1_Stream5->CR &= (~(1U<<26)) & (~(1U<<25));

	//Enable Memory address increment
	DMA1_Stream5->CR |= DMA_SCR_MINC;

	//Enabling Transfer Complete Interrupt
	DMA1_Stream5->CR |= DMA_SCR_TCIE;

	// Enable Circular mode
	DMA1_Stream5->CR |= DMA_SCR_CIRC;

	// Set transfer direction periph - mem
	DMA1_Stream5->CR &= (~(1U<<6)) & (~(1U<<7));

	// Enable DMA Stream
	DMA1_Stream5->CR |= DMA_SCR_EN;

	// Enable DMA Stream5 interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void dma1_stream6_uart_tx_config(uint32_t msg_to_send, uint32_t msg_len)
{
	// Disable DMA Stream
	DMA1_Stream6->CR &= ~DMA_SCR_EN;

	// Wait till DMA is disabled
	while((DMA1_Stream6->CR & DMA_SCR_EN)){}

	// Clear interrupt flags for Stream6
	DMA1->HIFCR |= HIFCR_CDMEIF6 | HIFCR_CTEIF6 | HIFCR_CTCIF6;

	// Set peripheral address
	DMA1_Stream6->PAR = (uint32_t)(&(USART2->DR));

	// Set memory address
	DMA1_Stream6->M0AR = msg_to_send;

	// Set number of transfer
	DMA1_Stream6->NDTR  = msg_len;

	// Select channel 4
	DMA1_Stream6->CR |= (1U<<27);
	DMA1_Stream6->CR &= (~(1U<<26)) & (~(1U<<25));

	//Enable Memory address increment
	DMA1_Stream6->CR |= DMA_SCR_MINC;

	// Set transfer direction mem - periph
	DMA1_Stream6->CR &= (~(1U<<7));
	DMA1_Stream6->CR |= (1U<<6);

	//Enabling Transfer Complete Interrupt
	DMA1_Stream6->CR |= DMA_SCR_TCIE;

	// Enable DMA Stream
	DMA1_Stream6->CR |= DMA_SCR_EN;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk +(baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

void DMA1_Stream5_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF5)
	{
		g_rx_complete = 1;

		/*{operation to be performed
		*
		*}*/
		// Clear the flag
		DMA1->HIFCR |= HIFCR_CTCIF5;
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF6)
	{
		// Operation to be performed
		g_tx_cmplt = 1;

		// Clear the flag
		DMA1->HIFCR |= HIFCR_CTCIF6;
	}
}

void USART2_IRQHandler(void)
{
	g_uart_complete = 1;

	// Clear TC flag interrupt
	USART2->SR &= ~SR_TC;

}
