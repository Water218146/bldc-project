#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "bsp_define.h"
#define LED_RCC_ENABLE()	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
#define LED_ON              Bit_SET
#define LED_OFF             Bit_RESET

typedef enum
{
    LED1 = 0,
    LED2,
    LED3,
    LED_MAX
} led_num_e;

extern uint8_t ucled[LED_MAX];

void led_task(void);
void bsp_led_init(void);
#endif