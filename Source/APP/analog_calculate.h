#ifndef __ANALOG_CALCULATE_H_
#define __ANALOG_CALCULATE_H_

#include "bsp_define.h"

typedef struct{
	//rule channel
	float v_bus;
	float temperature;
	float speed;
	//inject channle
	float current;
	float bemf_u;
	float bemf_v;
	float bemf_W;
}adc_voltage_val_e;

extern adc_voltage_val_e adc_voltage_val;

void adc_value_calculate(void);
	
void adc_calculate_task(void);
#endif