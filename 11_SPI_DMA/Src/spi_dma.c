#include "spi_dma.h"

#define GPIOAEN         (1U<<0)
#define SPI1EN          (1U<<12)

#define CR1SSM          (1U<<9)
#define CR1SSI          (1U<<8)
#define CR1MSTR         (1U<<2)
#define CR1CPOL         (1U<<1)
#define CR1CPHA         (1U<<0)
#define CR1SPE          (1U<<6)

#define CR2TXDMAEN      (1U<<1)
#define CR2RXDMAEN      (1U<<0)

#define DMA2EN			(1U<<22)

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

void spi1_dma_init(void)
{
    /************************GPIO CONFIGURATION************************/
    // Enable clock to port of SPI Pins
    RCC->AHB1ENR |= GPIOAEN;

    // Set SPI pins mode to alternate function mode
    /*  pin 5 ->    11  10
        pin 6 ->    13  12
        pin 7 ->    15  14
        1   0 ->    Alternate function mode*/
        
    GPIOA->MODER |= (1U<<11)    | (1U<<13)    | (1U<<15); // Setting 1 for MODER 11, 13, 15
    GPIOA->MODER &= (~(1U<<10)) & (~(1U<<12)) & (~(1U<<14)); // Setting 0 for MODER 10, 12, 14

    // Set alternate function type to AF5(SPI1)
    /*  pin 5 ->    23  22  21  20
        pin 6 ->    27  26  25  24
        pin 7 ->    31  30  29  28

        AF5   ->    0   1   0   1*/
    // Pin 5
    GPIOA->AFR[0] &= (~(1U<<23)) & (~(1U<<21));
    GPIOA->AFR[0] |= (1U<<22) | (1U<<20);
    // Pin 6
    GPIOA->AFR[0] &= (~(1U<<27)) & (~(1U<<25));
    GPIOA->AFR[0] |= (1U<<26) | (1U<<24);
    // Pin 7
    GPIOA->AFR[0] &= (~(1U<<31)) & (~(1U<<29));
    GPIOA->AFR[0] |= (1U<<30) | (1U<<28);
    

    /************************SPI CONFIGURATION************************/
    // Enable clock access to SPI1 modulde
    RCC->AHB2ENR |= SPI1EN;

    // Set software slave management
    SPI1->CR1 |= CR1SSM | CR1SSI;

    // Set SPI to master mode
    SPI1->CR1 |= CR1MSTR;

    // Set clock phase(CPHA) and clock polarity(CPOL)
    SPI1->CR1 |= SPI_CR1_BR_0 | CR1CPHA | CR1CPOL;

    // Set Clock devider : fpclk/4
    /*Baudrate control  ->  5   4   3
    for fpclk/4         ->  0   0   1*/
    SPI1->CR1 &= (~(1U<<5)) & (~(1U<<4));
    SPI1->CR1 |= (1U<<3);

    // Select to use DMA
    SPI1->CR2 |= CR2RXDMAEN | CR2TXDMAEN;

    // Enable SPI
    SPI1->CR1 |= CR1SPE;
}

void dma2_stream3_spi_tx_init(void)
{
    /******************************************************* DMA CONFIGURATION *******************************************************/
	// Enable clock access to DMA
    RCC->AHB1ENR |= DMA2EN;

    // Disable DMA
    DMA2_Stream3->CR = 0;

    // Wait till DMA is disabled
    while((DMA2_Stream3->CR & DMA_SCR_EN)){}

    /******************************************************* Configure DMA Stram parameters *******************************************************/
    // Enable memory address increment
    DMA2_Stream3->CR |= DMA_SCR_MINC;

    // Set transfer direction : memory to peripheral
    DMA2_Stream3->CR &= ~(1U<<7);
	DMA2_Stream3->CR |=  (1U<<6);

    // Enable transfer complete interrupt
    DMA2_Stream3->CR |= DMA_SCR_TCIE;

    // Enable transfer Error interrupt

    // Set DMA FIFO threshold

    // Enable DMA interrupt in NVIC
}