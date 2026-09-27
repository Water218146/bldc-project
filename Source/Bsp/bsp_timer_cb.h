#ifndef __BSP_TIMER_CB_H__
#define __BSP_TIMER_CB_H__

#include "bsp_define.h"

void bsp_timer8_irq_cb(void);
uint64_t timer_10us_get(void);

#endif