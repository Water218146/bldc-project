#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "bsp_define.h"
#define KEY_RCC_ENABLE()	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE)
#define KEY_INPUT_POLARTY   SET  //按键输入的极性 按键按下时是高电平还是低电平
#define KEY_BIT(key)    ((uint8_t)(1U << (key)))

typedef enum
{
	START_STOP_KEY = 0,
	CW_CCW_KEY,
	KEY_MAX
}key_num_e;

typedef struct
{
	uint8_t down_flag;
	uint32_t down_cnt;
}key_down_prama_t;

extern key_down_prama_t key_st_sp_prama;
extern key_down_prama_t key_cw_ccw_prama;

void key_task(void);
void bsp_key_init(void);
#endif