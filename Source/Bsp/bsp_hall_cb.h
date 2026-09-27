#ifndef __BSP_HALL_CB_H_
#define __BSP_HALL_CB_H_

#include "bsp_define.h"

void bsp_hall_u_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void));
void bsp_hall_v_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void));
void bsp_hall_w_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void));

extern void (*hall_uvw_irq_cb[3])(void (*bldc_sensor_algorithm_func_cb)(void));

#endif