#ifndef __ADXL345_H__
#define __ADXL345_H__

#include <stdint.h>
#include "spi_dma.h"

#define ADXL345_ADDR_DATA_FORMAT        0x31

#define ADXL345_DATAX0                  0x32
#define ADXL345_DATAX1                  0x33
#define ADXL345_DATAY0                  0x34
#define ADXL345_DATAY1                  0x35
#define ADXL345_DATAZ0                  0x36
#define ADXL345_DATAZ1                  0x37

#define ADXL345_SPI_4_WIRE              0x00
#define ADXL345_G_RANGE_2G              0x00
#define ADXL345_G_RANGE_4G              0x01
#define ADXL345_G_RANGE_8G              0x02
#define ADXL345_G_RANGE_16G             0x03

void axdl345_range_config(uint8_t mode);
void adxl345_accel_update(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream2_IRQHandler(void);
void axdl345_cs_pin_config(void);
void axdl345_cs_pin_set(void);
void axdl345_cs_pin_reset(void);
void adxl345_accel_update(void);
float adxl345_accel_get(uint8_t high_idx, uint8_t low_idx);
float adxl345_get_x(void);
float adxl345_get_y(void);
float adxl345_get_z(void);

#endif