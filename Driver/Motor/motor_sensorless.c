#include "motor_sensorless.h"



/**
  ******************************************************************************
  * @brief  电机无霍尔传感器模式下换相
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_sensorless_mode_phase(void)
{
#ifdef MOTOR_SENSORLESS_MODE		
	/*检测状态正常*/
	if((motor_ctrl_prama.error_sign == MOTOR_OPERATION_NORMAL) && (motor_ctrl_prama.motor_sta == MOTOR_START))
	{
		switch(motor_sl_prama.sensorless_state_machine)
		{
			case SENSORLESS_PRE_POSITION:  			//转子预定位
			{
				switch(motor_sl_prama.step)
				{
					case 0://转子预定位第1步：将转子预定位到U->VW位置
						mos_up_vnwn_phase(SENSORLESS_PRE_POSITION_STEP_1_PWM_DUTY);
						motor_sl_prama.step = 1;
					break;//预定位延时
					case 1:
						motor_sl_prama.timecnt++;
						if(motor_sl_prama.timecnt >= SENSORLESS_PRE_POSITION_STEP_1_PWM_TIME)
						{
							motor_sl_prama.step = 2;
							motor_sl_prama.timecnt = 0;
						}
					break;
					case 2://转子预定位第2步：将转子预定位到U->V或U->W位置
						
						if(motor_ctrl_prama.motor_direction == MOTOR_DIRECTION_CCW)
						{
							/*逆时针CCW方向:U+W-*/
							mos_up_wn_phase(SENSORLESS_PRE_POSITION_STEP_2_PWM_DUTY);
							motor_sl_prama.open_loop_phase_step = 1;   //初始位置默认为1  	U+W-
						}
						else
						{
							/*顺时针CW方向:U+V-*/
							mos_up_wn_phase(SENSORLESS_PRE_POSITION_STEP_2_PWM_TIME);
							motor_sl_prama.open_loop_phase_step = 1;   //初始位置默认为1  	U+V-
						}
						motor_sl_prama.step = 3;
					break;
					case 3://预定位延时
						motor_sl_prama.timecnt++;
						if(motor_sl_prama.timecnt >= SENSORLESS_PRE_POSITION_STEP_2_PWM_TIME)
						{
							motor_sl_prama.step = 0;
							motor_sl_prama.timecnt = 0;
							motor_sl_prama.sensorless_state_machine = SENSORLESS_OPEN_LOOP_SYNC_ACC;
							motor_sl_prama.open_loop_phase_freq = SENSORLESS_OPEN_LOOP_SYNC_ACC_MAX_TIME;	//初始换相频率赋值
							motor_sl_prama.open_loop_phase_voltage = SENSORLESS_OPEN_LOOP_SYNC_ACC_MIN_DUTY;//初始换相电压赋值
						}
					break;
				}
			}
			break;
			case SENSORLESS_OPEN_LOOP_SYNC_ACC:		//开环同步加速 open-loop synchronous acceleration
			{
				switch(motor_sl_prama.step)
				{
					case 0:
						/*换相频率改变*/
						motor_sl_prama.open_loop_phase_freq -= motor_sl_prama.open_loop_phase_freq / \
																	   SENSORLESS_OPEN_LOOP_SYNC_ACC_TIME_DIFF_VAL + 1;
						if(motor_sl_prama.open_loop_phase_freq < SENSORLESS_OPEN_LOOP_SYNC_ACC_MIN_TIME)
						{
							motor_sl_prama.open_loop_phase_freq = SENSORLESS_OPEN_LOOP_SYNC_ACC_MIN_TIME;
						}
						
						/*换相电压改变*/
						motor_sl_prama.open_loop_phase_voltage += motor_sl_prama.open_loop_phase_voltage * \
																		  SENSORLESS_OPEN_LOOP_SYNC_ACC_DUTY_DIFF_VAL + 1;
						if(motor_sl_prama.open_loop_phase_voltage > SENSORLESS_OPEN_LOOP_SYNC_ACC_MAX_DUTY)
						{
							motor_sl_prama.open_loop_phase_voltage = SENSORLESS_OPEN_LOOP_SYNC_ACC_MAX_DUTY;
						}
						/*开环换相*/
						motor_sl_prama.open_loop_phase_step++;//换相步进值+1
						if(motor_sl_prama.open_loop_phase_step > 6)
						{
							motor_sl_prama.open_loop_phase_step = 1;
						}
						
						motor_sensorless_open_loop_phase(motor_sl_prama.open_loop_phase_voltage);
						motor_sl_prama.step = 1;
						
					break;
					case 1:
						motor_sl_prama.timecnt++;
						if(motor_sl_prama.timecnt >= motor_sl_prama.open_loop_phase_freq)
						{
							motor_sl_prama.timecnt = 0;
							motor_sl_prama.step = 0;
						}
						/*是否进入闭环判断*/
						if(motor_sensorless_zero_algorithm() == 1)
						{
							motor_sl_prama.timecnt = 0;
							motor_sl_prama.step = 0;
							motor_sl_prama.sensorless_state_machine = SENSORLESS_CLOSE_LOOP_PHASE;
						}
					break;
					
				}
			}
			break;
			case SENSORLESS_CLOSE_LOOP_PHASE:      	//闭环换相
			{
				motor_sensorless_zero_algorithm();
			}
			break;
		}
	}
#endif	
}
