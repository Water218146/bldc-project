#include "motor_open_loop.h"

motor_run_key_t motor_run_key;

uint8_t ccw_motor_step = 1;
uint8_t cw_motor_step = 1;

/**
  ******************************************************************************
  * @brief  电机逆时针ccw开环运行
  * @param  duty：占空比
  * @retval None.
  ******************************************************************************/
void motor_open_ccw_operation(uint16_t duty)
{
		switch(ccw_motor_step)
		{
#if 1	
		/*
			骅融  WR-36BL61定子U、V、W三相线序定义方式为：
				U
		    |
				|
		   / \
		  /   \
		 V	   W 
		*/
			case 1:
				mos_up_vn_phase(duty);
			break;
			case 2:
				mos_up_wn_phase(duty);
			break;
			case 3:
				mos_vp_wn_phase(duty);					
			break;
			case 4:
				mos_vp_un_phase(duty);					
			break;
			case 5:
				mos_wp_un_phase(duty);					
			break;
			case 6:
				mos_wp_vn_phase(duty);					
			break;
		}
#else

#endif
		
		ccw_motor_step = (ccw_motor_step==6)?1:ccw_motor_step+1;
}

/**
  ******************************************************************************
  * @brief  电机顺时针cw开环运行
  * @param  duty：占空比
  * @retval None.
  ******************************************************************************/
void motor_open_cw_operation(uint16_t duty)
{
	switch(cw_motor_step)
	{
#if 1	
		/*
		
			骅融  WR-36BL61定子U、V、W三相线序定义方式为：
				U
		    |
				|
		   / \
		  /   \
		 V	   W 
		*/
		case 1:
			mos_vp_un_phase(duty);
		break;
		case 2:
			mos_vp_wn_phase(duty);
		break;
		case 3:
			mos_up_wn_phase(duty);
		break;
		case 4:
			mos_up_vn_phase(duty);
		break;
		case 5:
			mos_wp_vn_phase(duty);
		break;
		case 6:
			mos_wp_un_phase(duty);
		break;
#else
		
#endif
	}	
	cw_motor_step = (cw_motor_step==6)?1:cw_motor_step+1;
}

/* 电机开环控制任务 */
void motor_open_loop_task(void)
{
	/* 先判断电机运行状态 */
	if(motor_run_key.motor_start_stop == MOTOR_RUN)  //电机运行
	{
		if(motor_run_key.motor_cw_ccw == MOTOR_CCW)		//逆时针旋转
		{
			motor_open_ccw_operation(1000);		
		}
		else																					//顺时针旋转
		{
			motor_open_cw_operation(1000);
		}

	}
	else																		 				//电机停止
	{
		bsp_pwm_duty_set(0);
	}
}