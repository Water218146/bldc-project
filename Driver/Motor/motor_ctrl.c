/**
  ******************************************************************************
  * @file    motor_ctrl.c
  * @author  chengbb
  * @version V1.0
  * @date    2024-04-26
  * @brief   电机控制
  ******************************************************************************/
#include "motor_ctrl.h"

motor_ctrl_prama_t motor_ctrl_prama;

/* motor ctrl init */
void motor_ctrl_init()
{
	motor_ctrl_prama.pwm_duty = 0;	//	pwm清空
	motor_ctrl_prama.error_cnt = 0;	//错误计数清零
	motor_ctrl_prama.error_sign = MOTOR_OPERATION_IDLE;//电机错误标志置位空闲
}

/* 电机启动函数 返回1表示启动成功*/
int motor_start(uint16_t start_pwm_duty,uint8_t motor_dir)
{
#ifdef MOTOR_HALL_MODE			//电机霍尔模式执行
	int i =0;
#else							//电机非霍尔模式运行

#endif	
	/* 启动前进行系统检查 */
	if(motor_ctrl_prama.error_sign == MOTOR_OPERATION_FAULT)
	{
		motor_stop();//安全起见进行停机处理
		my_printf(DEBUG_COM,"motor_hall_sensor_mode:statr fault\r\n");
		return 0;
	}
	
	motor_ctrl_prama.pwm_duty = 0;		//初始占空比设置为0
	
	/* 自举电容充电第一步 关闭上下MOS管*/
	bsp_pwm_duty_set(motor_ctrl_prama.pwm_duty);
	MOS_UN_CTRL(Bit_RESET);
	MOS_VN_CTRL(Bit_RESET);
	MOS_WN_CTRL(Bit_RESET);
	
	/* Step2 Open 下管 */
	MOS_UN_CTRL(Bit_SET);
	MOS_VN_CTRL(Bit_SET);
	MOS_WN_CTRL(Bit_SET);
	
	/* Step3 延时一段时间，等待电容充满电 */
	bsp_delay_ms(BOOTSTRAP_BOOST_CHARGING_TIME);
	
	/* Step4 充电完成 Close下管 */
	MOS_UN_CTRL(Bit_RESET);
	MOS_VN_CTRL(Bit_RESET);
	MOS_WN_CTRL(Bit_RESET); 
	
#ifdef MOTOR_HALL_MODE			//霍尔模式下运行
	for(i=0;i<10;i++)
	{
		/* 检测霍尔传感器转子所在位置 */
		motor_get_hall_value();
		/*霍尔传感器出现全零或者全1*/
		if((hall_value.value == 7)||(hall_value.value==0))
		{
			motor_ctrl_prama.error_cnt++;
		}
	}
	/* 检测到错误数大于0 */
	if(motor_ctrl_prama.error_cnt>0)
	{
		motor_ctrl_prama.error_cnt = 0;	//错误计数清零
		motor_ctrl_prama.error_sign = MOTOR_OPERATION_FAULT; 
		motor_ctrl_prama.motor_sta = MOTOR_STOP;
		motor_stop();					//电机停机
		my_printf(DEBUG_COM,"hall_sensor_fault!!\r\n");
		return 0;
	}
	else
	{
		motor_ctrl_prama.error_sign = MOTOR_OPERATION_NORMAL;
	}
	
	motor_ctrl_prama.motor_direction = motor_dir;	//设置电机方向
	motor_ctrl_prama.pwm_duty = start_pwm_duty;		//设置电机启动占空比
	//如果启动状态正常
	if(motor_ctrl_prama.error_sign == MOTOR_OPERATION_NORMAL)
	{
		bsp_hall_irq_enable();//开启霍尔中断
		
		/* 根据电机方向进行初次的换向 */
		switch(motor_ctrl_prama.motor_direction)
		{
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
				my_printf(DEBUG_COM,"motor <motor_dir>prama:error\r\n");
			break;
		}
	}
#else					//电机无传感器模式执行
	motor_sensorless_init();
	motor_ctrl_prama.motor_direction = motor_dir; //电机启动方向
	motor_ctrl_prama.pwm_duty = start_pwm_duty;   //电机启动pwm占空比
	motor_ctrl_prama.error_sign = MOTOR_OPERATION_NORMAL;
#endif
	if(motor_ctrl_prama.motor_direction == MOTOR_DIRECTION_CCW)
	{
		my_printf(DEBUG_COM,"motor status:Motor Start,Motor Dir:CCW\r\n");
	}
	else
	{
		my_printf(DEBUG_COM,"motor status:Motor Start,Motor Dir:CW\r\n");		
	}
	return 1;//整个启动函数执行完毕 返回1
}

/**
  ******************************************************************************
  * @brief  motor stop
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_stop(void)
{
#ifdef MOTOR_HALL_MODE
	/* 上管3相关闭 */
	bsp_hall_irq_disable();					//关闭霍尔传感器
	motor_ctrl_prama.pwm_duty = 0;	//pwm占空比设置为0		
	bsp_pwm_duty_set(motor_ctrl_prama.pwm_duty);
	bsp_all_pwm_close();
	
	/* 下三管关闭 */
	MOS_UN_CTRL(Bit_RESET);
	MOS_VN_CTRL(Bit_RESET);
	MOS_WN_CTRL(Bit_RESET);
	motor_ctrl_prama.error_sign = MOTOR_OPERATION_IDLE;
#else
	motor_ctrl_prama.pwm_duty = 0;   				//pwm初始占空比设置为0
	bsp_pwm_duty_set(motor_ctrl_prama.pwm_duty);	//上管三相PWM占空比设置为0，IO输出为低电平，三相上桥MOS输出为0
	bsp_all_pwm_close();							//关闭PWM输出
	
	/*下管3相全部关闭*/
	MOS_UN_CTRL(Bit_RESET);							//下管U相MOS管脚为低电平
	MOS_VN_CTRL(Bit_RESET);							//下管V相MOS管脚为低电平		
	MOS_WN_CTRL(Bit_RESET);							//下管V相MOS管脚为低电平
	motor_ctrl_prama.error_sign = MOTOR_OPERATION_IDLE;
	
//	motor_sensorless_init();	
#endif
	// my_printf(DEBUG_COM,"motor status:Motor Stop\r\n");
	motor_ctrl_prama.calculate_speed = 0;				//停止时速度清零

}
