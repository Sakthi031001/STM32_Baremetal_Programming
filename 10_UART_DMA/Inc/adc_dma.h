#ifndef __ADC_DMA_H__
#define __ADC_DMA_H__

#include "stm32f446xx.h"
#include <stdint.h>

#define LISR_TCIF0 			(1U<<5)
#define LIFCR_CTCIF0		(1U<<5)

#define NUM_OF_CHANNELS 2
#define NUM_OF_SAMPLES 10

void adc_dma_init(void);
void adc_tim_dma_init(void);


#endif
