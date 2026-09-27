#include "motor_phase_tab.h"
#include "bsp_pwm.h"

/**
  ******************************************************************************
  * @file    motor_phase_tab.c
  * @author  chengbb
  * @version V1.0
  * @date    2024-04-16
  * @brief   电机换相表
  ******************************************************************************/

/**
  ******************************************************************************
  * @brief  mos u+v-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_up_vn_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC1EN));  	//U+ open
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));		//V+ close
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));		//W+ close
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_RESET);								//U- close
	MOS_VN_CTRL(Bit_SET);								//V- open
	MOS_WN_CTRL(Bit_RESET);								//W- close
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos u+w-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_up_wn_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC1EN));  	//U+ open
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));		//V+ close
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));		//W+ close
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_RESET);								//U- close
	MOS_VN_CTRL(Bit_RESET);								//V- close
	MOS_WN_CTRL(Bit_SET);								//W- open
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos v+u-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_vp_un_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC1EN));		//U+ close
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC2EN));		//V+ open
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));		//W+ close 
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_SET);								//U- open
	MOS_VN_CTRL(Bit_RESET);								//V- close
	MOS_WN_CTRL(Bit_RESET);								//W- close
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos v+w-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_vp_wn_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC1EN));		//U+ close
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC2EN));		//V+ open
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));		//W+ close 
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_RESET);								//U- close
	MOS_VN_CTRL(Bit_RESET);								//V- close
	MOS_WN_CTRL(Bit_SET);								//W- open
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos w+u-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_wp_un_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC1EN));		//U+ close
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));		//V+ close 
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC3EN));		//W+ open
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_SET);								//U- open
	MOS_VN_CTRL(Bit_RESET);								//V- close
	MOS_WN_CTRL(Bit_RESET);								//W- close
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos w+v-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_wp_vn_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC1EN));		//U+ close
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));		//V+ close 
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC3EN));		//W+ open
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_RESET);								//U- close
	MOS_VN_CTRL(Bit_SET);								//V- open
	MOS_WN_CTRL(Bit_RESET);								//W- close
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}

/**
  ******************************************************************************
  * @brief  mos u+v-w-
  * @param  duty:占空比设定
  * @retval None.
  ******************************************************************************/
void mos_up_vnwn_phase(uint16_t duty)
{
	uint16_t tmp;

	tmp = TIM1->CCEN;
	/*MOS上管PWM控制*/
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC1EN));  	//U+ open
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));		//V+ close
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));		//W+ close
	TIM1->CCEN = tmp;  //写入寄存器
	
	/*MOS下管IO控制*/
	MOS_UN_CTRL(Bit_RESET);								//U- close
	MOS_VN_CTRL(Bit_SET);								//V- open
	MOS_WN_CTRL(Bit_SET);								//W- open
	
	bsp_pwm_duty_set(duty);								//设置PWM占空比
}
