#include "adc_dma.h"

#define GPIOAEN			(1U<<0)
#define ADC1EN			(1U<<8)
#define CR1_SCAN		(1U<<8)
#define CR2_DMA			(1U<<8)
#define CR2_DDS			(1U<<9)
#define CR2_CONT		(1U<<1)
#define CR2_ADCON		(1U<<0)
#define CR2_SWSTART		(1U<<30)

#define DMA2EN			(1U<<22)
#define DMA_SCR_EN		(1U<<0)
#define DMA_SCR_MINC	(1U<<10)
#define DMA_SCR_PINC	(1U<<9)
#define DMA_SCR_CIRC	(1U<<8)
#define DMA_SCR_TCIE	(1U<<4)
#define DMA_SCR_TEIE	(1U<<2)
#define DMA_SFCR_DMDIS	(1U<<2)


uint16_t adc_raw_data[NUM_OF_CHANNELS];

void adc_dma_init(void)
{
	/******************************************************* GPIO CONFIGURATION *******************************************************/
	// Enable Clock access to the ADC GPIO pin's port
	RCC->AHB1ENR |= GPIOAEN;

	// Set PA0 and PA1 mode to analog mode
	GPIOA->MODER = GPIOA->MODER | (1U<<0) | (1U<<1);	//PIN 0
	GPIOA->MODER = GPIOA->MODER | (1U<<2) | (1U<<3);	//PIN 1

	/******************************************************* ADC CONFIGURATION *******************************************************/
	// Enable Clock access to the ADC
	RCC->APB2ENR |= ADC1EN;

	// Set sequence length
	ADC1->SQR1 |= (1U<<20);
	ADC1->SQR1  = ADC1->SQR1 & (~(1U<<21)) & (~(1U<<22)) & (~(1U<<23));

	// Set sequence
	ADC1->SQR3 = ADC1->SQR3 | (0U<<0) | (1U<<5);

	// Enable scan mode
	ADC1->CR1 |= CR1_SCAN;

	// Select to use DMA
	ADC1->CR2 = ADC1->CR2 | CR2_DMA | CR2_DDS | CR2_CONT;

	/******************************************************* DMA CONFIGURATION *******************************************************/
	// Enable clock access to DMA
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA
	DMA2_Stream0->CR &= ~DMA_SCR_EN;

	// Wait till DMA is disabled
	while((DMA2_Stream0->CR & DMA_SCR_EN)){}

	// Enable Circular mode
	DMA2_Stream0->CR |= DMA_SCR_CIRC;

	// Set Memory transfer size
	DMA2_Stream0->CR |=  (1U<<13);
	DMA2_Stream0->CR &= ~(1U<<14);

	// Set Peripheral transfer size
	DMA2_Stream0->CR |=	 (1U<<11);
	DMA2_Stream0->CR &= ~(1U<<12);

	// Enable Memory increment
	DMA2_Stream0->CR |= DMA_SCR_MINC;

	// Set peripheral address
	DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));

	// Set Memory address
	DMA2_Stream0->M0AR = (uint32_t)(&adc_raw_data);

	// Set number of transfer
	DMA2_Stream0->NDTR  = (uint16_t)(NUM_OF_CHANNELS);

	// Enable DMA Stream
	DMA2_Stream0->CR |= DMA_SCR_EN;

	/******************************************************* ADC CONFIGURATION *******************************************************/
	// Enable ADC
	ADC1->CR2 |= CR2_ADCON;

	// Start ADC
	ADC1->CR2 |= CR2_SWSTART;
}
