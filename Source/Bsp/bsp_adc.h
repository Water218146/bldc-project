#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_

#include "bsp_define.h"

typedef struct
{
	void (*adc_cb)(void (*u_formal_param)(void));
	void (*formal_param)(void);
}adc_irq_cb_t;

extern adc_irq_cb_t adc_irq_cb;

void bsp_adc_init(void (*irq_cb)(void (*formal_param)(void)), void (*formal_param)(void));

extern volatile uint16_t ADC_RegularConvertedValueTab[3];				//规则通道 DMA搬运使用 母线电压 温度 调速电位器
extern volatile uint16_t ADC_InjectedConvertedValueTab[4];			//注入通道 

#endif