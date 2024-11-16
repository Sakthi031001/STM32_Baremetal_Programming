#ifndef __ADXL345_H__
#define __ADXL345_H__

#include <stdint.h>
#include "spi_dma.h"

#define ADXL345_ADDR_DATA_FORMAT         0x31

#define ADXL345_SPI_4_WIRE               0x00
#define ADXL345_G_RANGE_2G               0x00
#define ADXL345_G_RANGE_4G               0x01
#define ADXL345_G_RANGE_8G               0x02
#define ADXL345_G_RANGE_16G              0x03

#define LISR_TCIF3                       (1U<<27)
#define LISR_TEIF3                       (1U<<25)

#define LIFCR_CTCIF3                     (1U<<27)
#define LIFCR_CTEIF3                     (1U<<25)

#define LISR_TCIF2                       (1U<<21)
#define LISR_TEIF2                       (1U<<19)

#define LIFCR_CTCIF2                     (1U<<21)
#define LIFCR_CTEIF2                     (1U<<19)

#endif