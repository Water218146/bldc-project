#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "bsp_define.h"

void bsp_systick_init(void);
void bsp_systick_disable(void);
uint32_t bsp_systick_get_tick(void);
void bsp_delay_ms(uint32_t ms);

extern volatile uint32_t n_tick;

#endif


