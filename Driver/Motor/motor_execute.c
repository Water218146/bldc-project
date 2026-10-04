#include "motor_execute.h"

/**
  ******************************************************************************
  * @file    motor_execute.c
  * @author  chengbb//
  * @version V1.0
  * @date    2026-09-22
  * @brief   电机执行
  ******************************************************************************/

motor_execute_state_machine_e motor_execute_state_machine = EXECUTE_IDLE;
pid_ctrl_tt motor_speed_pid_ctrl;

static uint8_t dir_change_flag = 0;				//电机换向标志
static uint8_t dir_change_status = 0;			//指示电机换向时的状态
static uint16_t target_pwm_duty = 0;			//目标转速pwm 由电位器控制
static uint8_t motor_direction = 0;				//电机方向

#define CHECK_INTERVAL_TIME			(100)       //间隔100ms检测
#define OVER_VOLTAGE_MAX_CNT		(5)       	//过压次数最大阈值
#define UNDER_VOLTAGE_MAX_CNT		(5)       	//欠压次数最大阈值
#define OVER_TEMPERATURE_MAX_CNT	(5)       	//过温次数最大阈值


/**
  ******************************************************************************
  * @brief  电机过压欠压检测
  * @param  None.
  * @retval None.
  ******************************************************************************/
 static void motor_over_vlotage_under_voltage_check(void)
 {
	static uint32_t check_time = 0;
	static uint8_t under_voltage_cnt = 0;
	static uint8_t over_voltage_cnt = 0;

	if(n_tick - check_time >= CHECK_INTERVAL_TIME)//100ms检查一次
	{
		check_time = n_tick;

		/* 检测到过压 */
		if(adc_voltage_val.v_bus > OVER_VOLTAGE_THRESHOLD_VALUE)
		{
			over_voltage_cnt++;//过压次数增加
			under_voltage_cnt = 0;//欠压次数清零
			if(over_voltage_cnt > OVER_VOLTAGE_MAX_CNT)//过压错误次数大于阈值
			{
				over_voltage_cnt = OVER_VOLTAGE_MAX_CNT;
				SET_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_VOLTAGE_ERROR);//标记为过压错误
			}
		}
		/* 检测到欠压 */
		else if(adc_voltage_val.v_bus < UNDER_VOLTAGE_THRESHOLD_VALUE)
		{
			under_voltage_cnt++;//欠压次数增加
			over_voltage_cnt = 0;	//过压次数清零
			if(under_voltage_cnt > UNDER_VOLTAGE_MAX_CNT)//欠压错误次数大于阈值
			{
				under_voltage_cnt = UNDER_VOLTAGE_MAX_CNT;
				SET_ERROR_TYPE(motor_ctrl_prama.error_type,UNDER_VOLTAGE_ERROR);//标记为欠压错误
			}
		}
		else//为检测到错误
		{
			/* 过压自愈机制 */
			if(over_voltage_cnt > 0)
			{
				over_voltage_cnt--;
			}
			else//没有过压错误了 清除过压错误标记
			{
				CLEAR_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_VOLTAGE_ERROR);
			}

			/* 欠压自愈机制 */
			if(under_voltage_cnt > 0)
			{
				under_voltage_cnt--;
			}
			else//清除欠压错误
			{
				CLEAR_ERROR_TYPE(motor_ctrl_prama.error_type,UNDER_VOLTAGE_ERROR);
			}
		}
	}
 }

/**
  ******************************************************************************
  * @brief  电机过温检测
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void motor_over_temperature_check(void)
{
	static uint32_t check_out = 0;
	static uint8_t over_temperature_cnt = 0;

	/* 100ms进行一次温度检测 */
	if(n_tick - check_out >= CHECK_INTERVAL_TIME)
	{
		check_out = n_tick;		//更新时间戳
		/* 温度大于阈值 */
		if(adc_voltage_val.temperature > OVER_TEMPERTURE_THRESHOLD_VALUE)
		{
			over_temperature_cnt++;
			/* 过温次数大于阈值（5次）*/
			if(over_temperature_cnt > OVER_TEMPERATURE_MAX_CNT)
			{
				over_temperature_cnt = OVER_TEMPERATURE_MAX_CNT;
				SET_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_TEMPERATURE_ERROR);//标记为过温错误
			}
		}
		/* 温度小于阈值 */
		else
		{
			/* 温度自愈机制 */
			if(over_temperature_cnt>0)
			{
				over_temperature_cnt--;
			}
			else//过温 次数为0之后 清除过温标志
			{
				CLEAR_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_TEMPERATURE_ERROR);
			}
		}
	}
}

/**
  ******************************************************************************
  * @brief  电机错误检测
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_error_check(void)
{
	static uint16_t error_sign_last = 0xffff;
	/* 调用电压错误检测函数 */
	motor_over_vlotage_under_voltage_check();
	/* 调用过温错误检测珊瑚 */
	motor_over_temperature_check();

	/* 错误状态发上变化时才进入判断 防止进行重复无效的判断 */
	if(motor_ctrl_prama.error_type != error_sign_last)
	{
		error_sign_last = motor_ctrl_prama.error_type;//记录历史错误状态

		/* 有错误 */
		if(motor_ctrl_prama.error_type != 0)		
		{
			motor_ctrl_prama.error_sign = MOTOR_OPERATION_FAULT;//标记为错误状态

			if(motor_ctrl_prama.motor_sta == MOTOR_START)
			{
				motor_stop();		//电机停止
				key_st_sp_prama.down_cnt = 0;//保证下次能够直接按下按键启动
				motor_ctrl_prama.error_sign = MOTOR_OPERATION_FAULT;//重新标记为错误状态防止错误状态被改变
			}
			/* 打印具体错误信息 */
			if(GET_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_VOLTAGE_ERROR))
			{	
				my_printf(DEBUG_COM,"check over voltage error\r\n");//打印过压错误
			}
			if(GET_ERROR_TYPE(motor_ctrl_prama.error_type,UNDER_VOLTAGE_ERROR))
			{	
				my_printf(DEBUG_COM,"check under voltage error\r\n");//打印低压错误
			}
			if(GET_ERROR_TYPE(motor_ctrl_prama.error_type,OVER_TEMPERATURE_ERROR))
			{	
				my_printf(DEBUG_COM,"check over temperature error\r\n");//打印过温错误
			}			
		}
		/* 状态从有错误变成了无错误 或 初级执行这个函数*/
		else
		{
			/*如果电机处于运行状态*/
			if(motor_ctrl_prama.motor_sta == MOTOR_START)
			{
				motor_ctrl_prama.error_sign = MOTOR_OPERATION_NORMAL;   //异常标记设置为normal
			}
			/*如果电机处于停机状态*/
			else
			{
				motor_ctrl_prama.error_sign = MOTOR_OPERATION_IDLE;		//异常标记设置为idle
			}
			my_printf(DEBUG_COM,"check normal\r\n");
		}
	}
}

/**
  ******************************************************************************
  * @brief  电机使用pwm调速 进行开环调速
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_open_speed(void)
{
	static uint32_t motor_speed_last_time = 0;
	static uint16_t adc_speed = 0;
	/* 电机方向没发生变化 输出pwm由电位器计算 */
	if(dir_change_flag==0)
	{
		#if 0	//不使用滤波算法
			target_pwm_duty = ((float)adc_digital_val.speed / 4095.0f) * MAX_PWM_DUTY;	.
		#else //使用滤波算法
			adc_speed = LPF_Calc(adc_digital_val.speed,adc_speed);
			target_pwm_duty = ((float)adc_speed / 4095.0f) * MAX_PWM_DUTY;
		#endif
	}
	if(target_pwm_duty < MOTOR_SENSORLESS_MODE_MIN_DUTY)
	{
		target_pwm_duty = MOTOR_SENSORLESS_MODE_MIN_DUTY;
	}
	/* 1ms调整一次转速 */
	if(n_tick - motor_speed_last_time >= 1)
	{
		motor_speed_last_time = n_tick;
		if(motor_ctrl_prama.pwm_duty < target_pwm_duty)//当期pwm小于目标pwm
		{
			motor_ctrl_prama.pwm_duty++;
		}
		if(motor_ctrl_prama.pwm_duty > target_pwm_duty)//当前pwm大于目标pwm
		{
			motor_ctrl_prama.pwm_duty--;
		}
	}
}

/**
  ******************************************************************************
  * @brief 速度pid初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
 void motor_pid_init(void)
 {
	memset(&motor_speed_pid_ctrl,0,sizeof(struct pid_ctrl_t));	//初始化PID控制结构体
#if POSITION_PID_CTRL
	//PID基础参数
	motor_speed_pid_ctrl.kp = PID_KP_GAIN;
	motor_speed_pid_ctrl.ki = PID_KI_GAIN;
	motor_speed_pid_ctrl.kd = PID_KD_GAIN;
	/* 初始化积分分离参数 */
	motor_speed_pid_ctrl.i_separate_p_threshold_value = PID_SEP_P_THRESHOLD_VALUE;	//积分分离正阈值
	motor_speed_pid_ctrl.i_separate_n_threshold_value = PID_SEP_N_THRESHOLD_VALUE;	//积分分离负阈值
	motor_speed_pid_ctrl.i_windup_p_threshold_value = PID_WINDUP_P_THRESHOLD_VALUE;	//积分限幅正阈值
	motor_speed_pid_ctrl.i_windup_n_threshold_value = PID_WINDUP_N_THRESHOLD_VALUE; //积分限幅负阈值
	
	motor_speed_pid_ctrl.uk_max_value = PID_UK_MAX_VALUE;	//输出最大值
	motor_speed_pid_ctrl.uk_min_value = PID_UK_MIN_VALUE;	//输出最小值
	
	motor_speed_pid_ctrl.error_p_band = PID_ERROR_P_BAND;	//正误差带
	motor_speed_pid_ctrl.error_n_band = PID_ERROR_N_BAND;	//负误差带
	motor_speed_pid_ctrl.pid_funtion = position_pid;		//pid执行对象：位置式pid
#else
	//PID基础参数
	motor_speed_pid_ctrl.kp = PID_KP_GAIN;
	motor_speed_pid_ctrl.ki = PID_KI_GAIN;
	motor_speed_pid_ctrl.kd = PID_KD_GAIN;

	//输出限幅值
	motor_speed_pid_ctrl.uk_max_value = PID_UK_MAX_VALUE;
	motor_speed_pid_ctrl.uk_min_value = PID_UK_MIN_VALUE;
	
	//指定pid算法
	motor_speed_pid_ctrl.pid_funtion = incremental_pid;	//增量式PID
#endif
 }
/**
  ******************************************************************************
  * @brief  电机pid算法速度环控制
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_pid_speed(void)
{
	static uint32_t timeout = 0;			//pid控制周期超时值
	static uint32_t adc_speed = 0;			//adc采集电位器数据原始值临时变量
	static uint32_t target_speed_value = 0;		//目标速度临时变量
	static uint32_t sampling_cycle = 0;		//pid控制采样周期
//该版本暂时只写有感模式
	uint8_t diff_val = 1;					//执行周期与采样周期的倍数

	/* 电机方向没发生改变 */
	if(dir_change_flag == 0)
	{
		/* 当前速度不为0 */
		if(motor_ctrl_prama.calculate_speed != 0)
		{
			/* 进行采样周期的计算 
			* 1min / 电机转速 = 电机 机械角度转了360° 所花的时间
			* 除去极对数 可得 电角度 旋转360° 所花的时间 
			* 电角度旋转360°所花的时间 就是采样周期 （霍尔传感器专属 霍尔传感器的采样时间 会根据电机实际转速而变化）
			**/
			sampling_cycle = M60_CONVERT_MS / motor_ctrl_prama.calculate_speed / MOTOR_PAIR_OF_POLES;
		}
		else	//	当前速度为0固定50ms采样
		{
			sampling_cycle = 50;
		}
		
		/* 执行周期(sampling_cycle * diff_val) 每周期进行PiD控制*/
		if((n_tick - timeout) >= (sampling_cycle * diff_val))
		{
			timeout = n_tick;
			/* 目标速度 */
			adc_speed = LPF_Calc(adc_digital_val.speed,adc_speed);		//adc采集电位器值进行滤波
			target_speed_value = adc_speed / 4095.0f * MOTOR_MAX_SPEED;	//电位器值缩放为目标速度
			motor_speed_pid_ctrl.target_value = LPF_Calc(target_speed_value,(uint32_t)motor_speed_pid_ctrl.target_value);//目标速度滤波

			/* 实时速度 */
			motor_speed_pid_ctrl.current_value = motor_ctrl_prama.calculate_speed;

			/* 调用PID控制函数 */
			motor_speed_pid_ctrl.pid_funtion(&motor_speed_pid_ctrl);	//经过pid控制算法后将速度误差转化为合理的PWM
			/* 更新输出pwm值 */
			motor_ctrl_prama.pwm_duty = motor_speed_pid_ctrl.uk_value;
		}
	}
	/* 电机方向发生了改变 */
	else
	{
		/*目标占空比最小值限幅*/
		if(target_pwm_duty < MOTOR_START_MIN_DUTY)
		{
			target_pwm_duty = MOTOR_START_MIN_DUTY;
		}
		
		/*间隔1ms进行一次pwm占空比调节*/
		if(bsp_systick_get_tick() - timeout >= 1)
		{
			timeout = bsp_systick_get_tick();
			if(target_pwm_duty > motor_ctrl_prama.pwm_duty)
			{
				motor_ctrl_prama.pwm_duty++;
			}
			else if(target_pwm_duty < motor_ctrl_prama.pwm_duty)
			{
				motor_ctrl_prama.pwm_duty--;
			}
		}
	}
	
}

/**
  ******************************************************************************
  * @brief  电机运行任务
  * @param  None.
  * @retval None.
  ******************************************************************************/
void motor_execute_task(void)
{
	/* 电机启停按键按下 */
	if(key_st_sp_prama.down_flag == 1)
	{
		key_st_sp_prama.down_flag = !key_st_sp_prama.down_flag;//消费这次按下的标志
		key_st_sp_prama.down_cnt++;
		if((key_st_sp_prama.down_cnt % 2) != 0)			
		{
			motor_ctrl_prama.motor_sta = MOTOR_START;//电机运行
		}
		else
		{
			motor_ctrl_prama.motor_sta = MOTOR_STOP;//电机停止
		}
	}
	
	/* 电机换向按键按下 */
	if(key_cw_ccw_prama.down_flag == 1)
	{
		key_cw_ccw_prama.down_flag = !key_cw_ccw_prama.down_flag;
		key_cw_ccw_prama.down_cnt++;
		if((key_cw_ccw_prama.down_cnt%2) != 0)
		{
			motor_direction = MOTOR_DIRECTION_CW;//电机顺时针旋转
			dir_change_flag = 1;
		}
		else
		{
			motor_direction = MOTOR_DIRECTION_CCW; //电机逆时针旋转
			dir_change_flag = 1;
		}
	}
	
	//电机进行了一次换向
	if(dir_change_flag)
	{
		/* 电机从运行过程中接收到换向指令 先让电机缓慢停下来 然后再换向 */
		if(motor_ctrl_prama.motor_sta == MOTOR_START)
		{
			target_pwm_duty = MOTOR_START_MIN_DUTY;//目标占空比设置为最小占空比
			switch(dir_change_status)
			{
				case 0:
					/* 等待电机占空比减到的合适的值后停止 */
					if(motor_ctrl_prama.pwm_duty < target_pwm_duty + 5)
					{
						motor_ctrl_prama.motor_sta = MOTOR_STOP;		//电机状态切换为停止
						dir_change_status = 1;
					}
				break;
				case 1:
					
				break;
			}
		}
		/* 电机在停止状态下 分为：运行到停止 和 始终是停止两种状态 */
		else
		{
			if(dir_change_status == 1)//是从运行状态切换切换过来的
			{
				dir_change_status = 0;
				dir_change_flag = 0;
				motor_ctrl_prama.motor_direction = motor_direction;
				motor_ctrl_prama.motor_sta = MOTOR_START;	//电机自动启动
				my_printf(DEBUG_COM,"#####motor_change_dir!\r\n");
			}
			else
			{
				dir_change_flag = 0;//清除换向标志位
				motor_ctrl_prama.motor_direction = motor_direction;//静止时也能切换方向
			}
		}
	}
	/* ----------- 转速日志输出 ------------ */
	#if 0
	static uint32_t speed_printf_time = 0;
	if(n_tick - speed_printf_time >= 500)
	{
		speed_printf_time = n_tick;//更新时间
		my_printf(DEBUG_COM,"motor_speed:%d RPM\r\n",motor_ctrl_prama.calculate_speed);
	}ne 
	#endif

	/* ----------- VOFA+可视化参数观测与调试（使用HOST_COMPUTER_COM） ------------ */
	static uint32_t pc_timeout = 0;
	if(bsp_systick_get_tick() - pc_timeout >= 10)//10ms进行一次输出
	{
		pc_timeout = bsp_systick_get_tick();
		my_printf(HOST_COMPUTER_COM,"motor_ctrl_prama:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",motor_speed_pid_ctrl.target_value,motor_speed_pid_ctrl.current_value,motor_speed_pid_ctrl.p_value,motor_speed_pid_ctrl.i_value,motor_speed_pid_ctrl.kp,motor_speed_pid_ctrl.ki,motor_speed_pid_ctrl.kd,(float)motor_ctrl_prama.pwm_duty);
	}

	/*  */
	/*===================执行错误检测函数==================*/
	motor_error_check();	
	/*===================电机运行状态机====================*/
	switch(motor_execute_state_machine)
	{
		/* 电机处于空闲状态 */
		case EXECUTE_IDLE:		
			if(motor_ctrl_prama.motor_sta == MOTOR_START)
			{
				motor_execute_state_machine = EXECUTE_MOTOR_START;
			}
		break;
		/* 电机处于启动状态 */
		case EXECUTE_MOTOR_START:
		{
			int ret = 0;
			motor_pid_init();
			// motor_start(MOTOR_START_MIN_DUTY,motor_ctrl_prama.motor_direction);		//执行一次强拖换向 触发霍尔中断
			ret = motor_start(MOTOR_START_MIN_DUTY,motor_ctrl_prama.motor_direction);//执行一次强拖换向，且进行启动时的安全检测
			// if(motor_ctrl_prama.error_sign == MOTOR_OPERATION_FAULT)	//检测到了错误
			if(ret == 0)//启动错误
			{
				motor_execute_state_machine = EXECUTE_MOTOR_STOP;
				key_st_sp_prama.down_cnt = 0;//按键清零 让再次按下启动按键时 能直接进行启动
				my_printf(DEBUG_COM,"motor_start:flaut!!\r\n");
			}
			else
			{
				motor_execute_state_machine = EXECUTE_MOTOR_EXECUTE;
			}
			break;

		}

		/* 电机处于运行状态 */
		case EXECUTE_MOTOR_EXECUTE:
			// motor_open_speed();					//电机速度调整 开环
			motor_pid_speed();
			if(motor_ctrl_prama.motor_sta == MOTOR_STOP)
			{
				motor_stop();
				/* 状态切换为STOP状态 */
				motor_execute_state_machine = EXECUTE_MOTOR_STOP;
			} 
		break;
		/* 电机处于停止状态 */
		case EXECUTE_MOTOR_STOP:
			motor_execute_state_machine = EXECUTE_IDLE;			//直接切换到空闲状态
		break;
	}
	
}
