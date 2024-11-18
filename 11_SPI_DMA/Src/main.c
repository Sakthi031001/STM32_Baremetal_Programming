#include "stm32f446xx.h"
#include "spi_dma.h"
#include "ADXL345.h"

float acc_x, acc_y, acc_z;


int main ()
{

	// Enable SPI
	spi1_dma_init();

	// Config cs pin
	axdl345_cs_pin_config();

	// Enable floating point unit
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

	// Enable tx stream
	dma2_stream3_spi_tx_init();

	// Enable rx stream
	dma2_stream2_spi_rx_init();

	// Reset cs pin
	axdl345_cs_pin_reset();

	// Config accel
	axdl345_range_config(ADXL345_G_RANGE_2G);

	while(1)
	{
		// Reset CS pin
		axdl345_cs_pin_reset();

		// Get Accel data
		acc_x = adxl345_get_x();
		acc_y = adxl345_get_y();
		acc_z = adxl345_get_z();

		// Set NCS pin
		axdl345_cs_pin_set();
	}
}
