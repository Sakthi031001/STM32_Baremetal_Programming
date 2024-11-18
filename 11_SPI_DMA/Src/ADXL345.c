#include "ADXL345.h"

#define SPI_DATA_BUFF_LEN               2

#define ADXL345_SPI                     (1U<<6)

double g_range;

#define MAX_TRANSFER_LEN                6
#define READ_FLAG                       0x80
#define GPIOAEN                         (1U<<0)
uint8_t spi_data_buff[2];
uint8_t g_tx_cmplt;
uint8_t g_rx_cmplt;
uint8_t dummy_buff[MAX_TRANSFER_LEN+1];
uint8_t accel_buff[MAX_TRANSFER_LEN+1];

void axdl345_cs_pin_config(void)
{
    RCC->AHB1ENR |= GPIOAEN;

    // Set PA0 as output pin
    GPIOA->MODER |= (1U<<0);
    GPIOA->MODER &= ~(1U<<1);
}

void axdl345_cs_pin_set(void)
{
    GPIOA->ODR |= (1U<<0);
}

void axdl345_cs_pin_reset(void)
{
    GPIOA->ODR &= ~(1U<<0);
}


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

    dma2_stream3_spi_transfer((uint32_t) spi_data_buff[0], (uint32_t) SPI_DATA_BUFF_LEN);

    // Wait for transfer completion
    while(!g_tx_cmplt){}

    // Reset flag
    g_tx_cmplt = 0;

}

void adxl345_accel_update(void)
{
    dummy_buff[0] = ADXL345_DATAX1 | READ_FLAG;

    dma2_stream2_spi_receive((uint32_t) accel_buff, (uint32_t) (MAX_TRANSFER_LEN+1));

    dma2_stream3_spi_transfer((uint32_t)dummy_buff, (uint32_t) (MAX_TRANSFER_LEN+1));

    // Wait for transfer completion
    while(!g_rx_cmplt){}

    // Reset flag
    g_rx_cmplt = 0;
}

float adxl345_accel_get(uint8_t high_idx, uint8_t low_idx)
{
    int16_t rslt;
    rslt = accel_buff[high_idx]<<8 | accel_buff[low_idx];
    if(rslt)
    {
        return((float)- rslt) * g_range/ (float) 0x8000;
    }
    else{
        return 0.0;
    }
}

float adxl345_get_x(void)
{
    adxl345_accel_get(1, 2);
    return 0.0;
}

float adxl345_get_y(void)
{
    adxl345_accel_get(3, 4);
    return 0.0;
}

float adxl345_get_z(void)
{
    adxl345_accel_get(5, 6);
    return 0.0;
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
