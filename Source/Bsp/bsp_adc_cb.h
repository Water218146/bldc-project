#ifndef __BSP_ADC_CB_H_
#define __BSP_ADC_CB_H_

#include "bsp_define.h"

typedef struct
{
	uint16_t v_bus;
	uint16_t temperature;
	uint16_t speed;
	
	uint16_t current;
	uint16_t bemf_u;
	uint16_t bemf_v;
	uint16_t bemf_w;
}adc_digital_val_e;

extern adc_digital_val_e adc_digital_val;

void bsp_adc_irq_cb(void (*bldc_sensorless_algorithm_func_cb)(void));


#endif 