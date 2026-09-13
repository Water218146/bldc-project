#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "bsp_define.h"

#define MOS_UN_CTRL(level)		GPIO_WriteBit(GPIOB, GPIO_PIN_13, level)
#define MOS_VN_CTRL(level)		GPIO_WriteBit(GPIOB, GPIO_PIN_14, level)
#define MOS_WN_CTRL(level)		GPIO_WriteBit(GPIOB, GPIO_PIN_15, level)

#define PWM_PERIOD_MAX			(2700)		//pwm period最大值

typedef struct
{
	void (*pwm_bk_cb)(void);
	void (*pwm_cb)(void);
}pwm_irq_cb_t;

extern pwm_irq_cb_t pwm_irq_cb;

void bsp_pwm_init(void (*irq_bk_cb)(void), void (*irq_cb)(void));
void bsp_pwm_duty_set(uint16_t duty);
void bsp_all_pwm_open(void);
void bsp_all_pwm_close(void);
void bsp_set_pwm_freq(uint16_t freq);

#endif