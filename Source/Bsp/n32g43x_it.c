/**
 * @file n32g43x_it.c
 * @author N32cube
 */

 /* NTFx CODE START */
#include "n32g43x_it.h"
#include "n32g43x.h"
#include "bsp_define.h"
/* NTFx CODE END */

/* NTFx CODE START */
extern __IO uint32_t n_tick;
/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
/* NTFx CODE END */

}
/* NTFx CODE START */
/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    /* NTFx CODE END */

    }
}
/* NTFx CODE START */
/**
 * @brief  This function handles Memory Manage exception.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
/* NTFx CODE END */

    }
}
/* NTFx CODE START */
/**
 * @brief  This function handles Bus Fault exception.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
/* NTFx CODE END */

    }
}
/* NTFx CODE START */
/**
 * @brief  This function handles Usage Fault exception.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
/* NTFx CODE END */

    }
}
/* NTFx CODE START */
/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
/* NTFx CODE END */

}
/* NTFx CODE START */
/**
 * @brief  This function handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
/* NTFx CODE END */

}
/* NTFx CODE START */
/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
   n_tick++;
/* NTFx CODE END */

}

/* DEBUG_COM IRQ */
void UART4_IRQHandler(void)
{
	if (com_irq_cb.debug_com_cb != NULL)
	{
		com_irq_cb.debug_com_cb();
	}
}

/* HOST_COMPUTER_COM IRQ*/
void UART5_IRQHandler(void)
{
	if (com_irq_cb.host_computer_com_cb != NULL)
	{
		com_irq_cb.host_computer_com_cb();
	}
}

/* RS485_COM IRQ*/
void USART3_IRQHandler(void)
{
	if (com_irq_cb.rs485_com_cb != NULL)
	{
		com_irq_cb.rs485_com_cb();
	}
}

/* ADC IRQ */
void ADC_IRQHandler(void)
{
	adc_irq_cb.adc_cb(adc_irq_cb.formal_param);
}

/* EXTI1 IRQ */
void EXTI1_IRQHandler(void)
{
	hall_irq_cb.hall_u_cb(hall_irq_cb.formal_param);//HALL U相的中断函数
}

/* EXTI2 IRQ */
void EXTI2_IRQHandler(void)
{
	hall_irq_cb.hall_v_cb(hall_irq_cb.formal_param);//HALL V相的中断函数
}

/*EXTI3 IRQ */
void EXTI3_IRQHandler(void)
{
	hall_irq_cb.hall_w_cb(hall_irq_cb.formal_param);//HALL W相的中断函数
}

/* TIM8 up Irq */
void TIM8_UP_IRQHandler(void)
{
	timer8_irq_cb.timer_cb();
}

/* Break Irq */
volatile uint32_t BRK_CNT = 0;
void TIM1_BRK_IRQHandler(void)
{
	pwm_irq_cb.pwm_bk_cb();
	BRK_CNT++;
}