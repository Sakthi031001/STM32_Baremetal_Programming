#include "ADXL345.h"

#define SPI_DATA_BUFF_LEN               2

#define ADXL345_SPI                     (1U<<6)

double g_range;

uint8_t spi_data_buff[2];
uint8_t g_tx_cmplt;
uint8_t g_rx_cmplt;

void axdl345_range_config(uint8_t mode)
{
    switch(mode)
    {
        case ADXL345_G_RANGE_2G:
            g_range = 2.0;
            break;
        
        case ADXL345_G_RANGE_4G:
            g_range = 4.0;
            break;

        case ADXL345_G_RANGE_8G:
            g_range = 8.0;
            break;
        
        case ADXL345_G_RANGE_16G:
            g_range = 16.0;
            break;
        
        default:
            break;
    }

    // Set to SPI mode
    spi_data_buff[0] = ADXL345_ADDR_DATA_FORMAT;

    dma2_stream3_spi_transfer((uint32_t) spi_data_buff[0], (uint32_t) SPI_DATA_BUFF_LEN);

    // Wait for transfer completion
    while(!g_tx_cmplt){}

    // Reset flag
    g_tx_cmplt = 0;

    // Configure the RANGE
    spi_data_buff[0] = ADXL345_ADDR_DATA_FORMAT;

}

void DMA2_Stream3_IRQHandler(void)
{
    if(DMA2->LISR & LISR_TCIF3)
    {
        // Operation to be performed
        g_tx_cmplt = 1;

        // Clear the flag
        DMA1->LIFCR |= LIFCR_CTCIF3;
    }
    else if(DMA2->LISR & LISR_TEIF3)
    {
        // Operation to be performed

        // Clear the flag
        DMA1->LIFCR |= LIFCR_CTEIF3;
    }
}

void DMA2_Stream2_IRQHandler(void)
{
    if(DMA2->LISR & LISR_TCIF2)
    {
        // Operation to be performed
        g_rx_cmplt = 1;

        // Clear the flag
        DMA1->LIFCR |= LIFCR_CTCIF2;
    }
    else if(DMA2->LISR & LISR_TEIF2)
    {
        // Operation to be performed

        // Clear the flag
        DMA1->LIFCR |= LIFCR_CTEIF2;
    }
}