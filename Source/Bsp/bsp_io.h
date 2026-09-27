#ifndef __BSP_IO_H_
#define __BSP_IO_H_

#include "bsp_define.h"

#define ADC_TEST_IO_HIGH() GPIO_SetBits(GPIOC,GPIO_PIN_5)
#define ADC_TEST_IO_LOW()  GPIO_ResetBits(GPIOC,GPIO_PIN_5)

void bsp_io_init(void);

#endif