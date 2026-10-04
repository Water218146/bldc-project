#include "motor_sensor.h"

hall_value_t hall_value;
uint8_t hall_buf[6];
static uint8_t h_index = 0;

/**
  ******************************************************************************
  * @brief  获取霍尔传感器值
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_get_hall_value(void)
{
	hall_value.u_value = HALL_U_IO_VALUE();
	hall_value.v_value = HALL_V_IO_VALUE();
	hall_value.w_value = HALL_W_IO_VALUE();
	
	hall_value.value = ((hall_value.u_value<<2)|(hall_value.v_value<<1)|(hall_value.w_value<<0)) & 0x7f;
	
	/* ================ 进行速度计算 =============== */

	/* 查找上升沿 */
	if(hall_value.u_value == 1)
	{
		if(hall_value.level_sign == 0)
		{
			hall_value.rising_time = timer_10us_get();
			hall_value.level_sign = 1;	//成功查找到上升沿
		}
	}
	/* 查找下降沿 */
	if(hall_value.u_value == 0)
	{
		if(hall_value.level_sign == 1)
		{
			hall_value.falling_time = timer_10us_get();
			hall_value.level_sign = 2;	//成功找到下降沿
		}
	}
	/* 上升下降沿都找到了 */
	if(hall_value.level_sign == 2)
	{
	#if 0			 /* --------开环速度计算---------- */
		static uint64_t time = 0;
		static uint64_t timef = 0;
		
		
		hall_value.level_sign = 0; 	//清空采集标志
		time = hall_value.falling_time - hall_value.rising_time;	//此时单位为10us
		time = time *2;							//计算电周期
		time = time * MOTOR_PAIR_OF_POLES;		//乘电机极对数 即为电机机械角度旋转一圈所花的时间		
		time = time * 10;						//放大10倍 单位转成1us
		time = time / 1000;					//单位转化为ms
		
		timef = LPF_Calc(time,timef);					//滤波算法 LPF_Calc(Xin,Yout)		((Yout>>1)+(Yout>>2)+(Xin>>2)) time:当前时间 timef:上次滤波后的值
		/* 60000ms时间除以电机旋转1全所需要的值(ms) 即可得到电机转速rpm */
		hall_value.sp_filter = M60_CONVERT_MS / timef;	//60000 / t
		motor_ctrl_prama.calculate_speed = LPF_Calc(hall_value.sp_filter,motor_ctrl_prama.calculate_speed);
		
	#else			/* 闭环PID控制使用速度计算方法 */	 
		static uint64_t time = 0;
		static uint64_t timef = 0;
		static uint64_t timef1 = 0;
		static uint64_t calculate_speed_f;
		hall_value.level_sign = 0;
		
		time = hall_value.falling_time - hall_value.rising_time;	//计算出高电平区间时间   180°电角度对应时间
		time = time * 2; 											//计算出电周期(360°电角度)时间
		time = time * MOTOR_PAIR_OF_POLES;							//根据极对数计算出旋转360°机械角度所需时间(也就是转子旋转一圈时间)
		time = time * 10;							//将10us单位转成1us单位
		/* 时间计算双重滤波，稳定性和精度更高 */
		timef = LPF_Calc(time, timef);   			//time:就是当前计算的值   timef:上次滤波后的值
		timef1 = LPF_Calc(timef, timef1);   		//timef1:就是当前计算的值  timef:上次滤波后的值
		/*60000000us时间值除以电机旋转1圈所需要的时间(us)，就可以计算出电机1min旋转了多少转*/
		hall_value.sp_filter = (M60_CONVERT_MS * 1000) / timef1;	//转换为us单位计算，精度更高，更适合pid算法闭环控制，要不然转速有滞后	
		calculate_speed_f = LPF_Calc(hall_value.sp_filter, calculate_speed_f);
		motor_ctrl_prama.calculate_speed = LPF_Calc(calculate_speed_f, motor_ctrl_prama.calculate_speed);		
	#endif  
	}
	

}
	
/**
  ******************************************************************************
  * @brief  电机带霍尔传感器模式下换相
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_sensor_mode_phase(void)
{
	motor_get_hall_value();
	hall_buf[h_index++] = hall_value.value;
	h_index = h_index % 6;
	/* 获取换向时间 */
	motor_ctrl_prama.motor_phase_time = bsp_systick_get_tick();
	if((motor_ctrl_prama.error_sign == MOTOR_OPERATION_NORMAL) && (motor_ctrl_prama.motor_sta == MOTOR_START))//电机状态无错误且当前是运行状态
	{
		switch(motor_ctrl_prama.motor_direction)
		{
			/*
			
				骅融  WR-36BL61定子U、V、W三相线序定义方式为：
					U
					|
					|
				 / \
				/   \
			 V	   W 
			
			*/
			/*
			HALL值逆时针CCW：2      3      1      5      4      6
			换相值		   : u+v-   u+w-   v+w-   v+u-   w+u-   w+v-
			*/
			case MOTOR_DIRECTION_CCW:
				switch(hall_value.value)
				{
					case 2:
						mos_up_vn_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 3:
						mos_up_wn_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 1:
						mos_vp_wn_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 5:
						mos_vp_un_phase(motor_ctrl_prama.pwm_duty);						
					break;
					case 4:
						mos_wp_un_phase(motor_ctrl_prama.pwm_duty);						
					break;
					case 6:
						mos_wp_vn_phase(motor_ctrl_prama.pwm_duty);						
					break;
				}
			break;
			/*
			HALL值顺时针CW：2      6      4      5      1      3
			换相值		   : v+u-   v+w-   u+w-   u+v-   w+v-   w+u-
			*/
			case MOTOR_DIRECTION_CW:	
				switch(hall_value.value)
				{
					case 2:
						mos_vp_un_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 6:
						mos_vp_wn_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 4:
						mos_up_wn_phase(motor_ctrl_prama.pwm_duty);
					break;
					case 5:
						mos_up_vn_phase(motor_ctrl_prama.pwm_duty);						
					break;
					case 1:
						mos_wp_vn_phase(motor_ctrl_prama.pwm_duty);						
					break;
					case 3:
						mos_wp_un_phase(motor_ctrl_prama.pwm_duty);						
					break;
				}				
			break;
			
			default:
				
			break;
		}
	}
}

