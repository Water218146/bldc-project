#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "bsp_define.h"

typedef struct
{
	void (*timer_cb)(void);
}timer_irq_cb_t;

extern timer_irq_cb_t timer8_irq_cb;

void bsp_timer8_init(void (*irq_cb)(void));

#endif
