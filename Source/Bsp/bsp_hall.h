#ifndef __BSP_HALL_H_
#define __BSP_HALL_H_

#include "bsp_define.h"

#define HALL_U_IO_VALUE()			GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_1)
#define HALL_V_IO_VALUE()			GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_2)
#define HALL_W_IO_VALUE()			GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_3)

typedef struct{
	void (*hall_u_cb)(void(*u_formal_param)(void));
	void (*hall_v_cb)(void(*u_formal_param)(void));
	void (*hall_w_cb)(void(*w_formal_param)(void));
	
	void (*formal_param)(void);
}hall_irq_cb_t;

extern hall_irq_cb_t hall_irq_cb;

void bsp_hall_irq_enable(void);
void bsp_hall_irq_disable(void);
void bsp_hall_init(void (*irq_cb[3])(void (*formal_param)(void )),void (*formal_param)(void));


void hall_sensor_test(void);

#endif