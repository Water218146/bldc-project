#ifndef __MOTOR_CTRL_H
#define __MOTOR_CTRL_H

#include "bsp_define.h"

#define MOTOR_HALL_MODE		//电机在HALL模式执行

#ifndef MOTOR_HALL_MODE
#define MOTOR_SENSORLESS_MODE	//无霍尔传感器模式执行
#endif

//#define POSITION_PID_CTRL			//位置式PID控制
#ifndef POSTION_PID_CTRL	
#define INCREMENTAL_PID_CTRL		//增量式PID控制
#endif

#define MOTOR_POLES_MUMBER			(4)													//电机极数
#define MOTOR_PAIR_OF_POLES			(MOTOR_POLES_MUMBER / 2)		//电机极对数
#define M60_CONVERT_MS					(60 * 1000)									//将60s转为60000ms

/*
滤波算法分析：
(Yout>>1)：将Yout值减小1/2
(Yout>>2)：将Yout值减小1/4
所以：Yout值取3/4         //100     50 + 25 = 75
(Xin>>2)：将Xin值减小1/4  //100     25
所以整体输出值为：Yout * 3/4 + Xin * 1/4;
总结：相当于是一个消除尖峰和低谷滤波的算法
*/
#define LPF_Calc(Xin,Yout)							((Yout>>1)+(Yout>>2)+(Xin>>2))			//Xn:in		Yn:out

#define OVER_VOLTAGE_THRESHOLD_VALUE		(28.0f)															//母线电压过压阈值
#define UNDER_VOLTAGE_THRESHOLD_VALUE		(20.0f)															//母线电压欠压阈值
#define OVER_TEMPERTURE_THRESHOLD_VALUE	(40.0f)															//过温阈值 >=70

#define OVER_VOLTAGE_ERROR							(0x0001)														//bit0:过压
#define UNDER_VOLTAGE_ERROR							(0x0002)														//bit1:欠压
#define OVER_TEMPERATURE_ERROR					(0x0004)														//bit2:过温

#define SET_ERROR_TYPE(variable,type)		variable|= type											//设置异常状态
#define CLEAR_ERROR_TYPE(variable,type) variable &= ~(type)									//清除异常状态
#define GET_ERROR_TYPE(variable,type)		(variable & type)										//获取异常状态

#ifdef MOTOR_HALL_MODE 			//霍尔传感器模式下
#define	MOTOR_MAX_SPEED									(2500.0f)														//电机最大转速2500rpm
#define MOTOR_MIN_SPEED									(500.0f)														//电机最小转速500rpm

#else					//无霍尔传感器模式下
#define MOTOR_MAX_SPEED								(3600)																//电机最大转速3600rpm
#define MOTOR_MIN_SPEED								(700)																	//电机最小转速700rpm
#endif

#ifdef POSTION_PID_CTRL		//位置式PID控制参数
#define PID_KP_GAIN											(0.035f)														//PID控制比例增益
#define PID_KI_GAIN											(0.01f)															//PID控制积分项增益
#define PID_KD_GAIN											(0.0f)															//PID控制微分项增益
#define PID_SEP_P_THRESHOLD_VALUE				(10.0f)															//积分分离正阈值
#define PID_SEP_N_THRESHOLD_VALUE				(-10.0f)														//积分分离负阈值
#define PID_WINDUP_P_THRESHOLD_VALUE		(100.0f)														//积分限幅正阈值
#define PID_WINDUP_N_THRESHOLD_VALUE		(-100.0f)														//积分限幅负阈值
#define PID_UK_MAX_VALUE								((float)MAX_PWM_DUTY)								//输出最大值
#define PID_UK_MIN_VALUE								((float)MIN_PWM_DUTY)								//输出最小值
#define PID_ERROR_P_BAND								(2.0f)															//正误差带
#define PID_ERROR_N_BAND								(-2.0f)															//负误差带
#else						//增量式PID控制参数

#ifdef	MOTOR_HALL_MODE	//霍尔传感器模式下
#define PID_KP_GAIN									(0.4f)																	//PID控制比例项增益 0.005f    //ATK-BL57H95D20E 0.1f
#define PID_KI_GAIN									(0.04f)																	//PID控制积分项增益 0.03f  	  //ATK-BL57H95D20E 0.02f 
#define PID_KD_GAIN									(0.0f)																	//PID控制微分项增益
#define PID_UK_MAX_VALUE						((float)MAX_PWM_DUTY)										//输出最大值
#define PID_UK_MIN_VALUE						((float)PWM_20_DUTY)										//输出最小值
#else					//无霍尔传感器模式下
#define PID_KP_GAIN									(0.2f)																	//PID控制比例项增益
#define PID_KI_GAIN									(0.02f)																	//PID控制积分项增益
#define PID_KD_GAIN									(0.0f)																	//PID控制微分项增益
#define PID_UK_MAX_VALUE						((float)MAX_PWM_DUTY)										//输出最大值
#define PID_UK_MIN_VALUE						((float)PWM_20_DUTY)										//输出最小值	
#endif

#endif

#define BOOTSTRAP_BOOST_CHARGING_TIME			200			//自举升压充电时间
#define MOTOR_DIRECTION_CCW								0				//电机运行方向，逆时针
#define MOTOR_DIRECTION_CW								1				//电机运行发相，顺时针

#define HALL_ERROR_VALUE_0								0				//霍尔传感器错误值，0
#define HALL_ERROR_VALUE_7								1				//霍尔传感器错误值，1

#define MOTOR_OPERATION_IDLE							0				//电机空闲状态
#define MOTOR_OPERATION_FAULT							1				//电机运行错误
#define MOTOR_OPERATION_NORMAL						2				//电机运行正常

#define MOTOR_STOP									0									//电机运行状态：停止						
#define MOTOR_START									1									//电机运行状态：启动

#define PWM_5_DUTY									(PWM_PERIOD_MAX * 0.05)				//PWM10%占空比
#define PWM_10_DUTY									(PWM_PERIOD_MAX * 0.1)				//PWM10%占空比
#define PWM_20_DUTY									(PWM_PERIOD_MAX * 0.2)				//PWM20%占空比
#define PWM_30_DUTY									(PWM_PERIOD_MAX * 0.3)				//PWM30%占空比
#define PWM_40_DUTY									(PWM_PERIOD_MAX * 0.4)				//PWM40%占空比
#define PWM_50_DUTY									(PWM_PERIOD_MAX * 0.5)				//PWM50%占空比
#define PWM_60_DUTY									(PWM_PERIOD_MAX * 0.6)				//PWM60%占空比
#define PWM_70_DUTY									(PWM_PERIOD_MAX * 0.7)				//PWM70%占空比
#define PWM_80_DUTY									(PWM_PERIOD_MAX * 0.8)				//PWM80%占空比
#define PWM_90_DUTY									(PWM_PERIOD_MAX * 0.9)				//PWM90%占空比
#define PWM_95_DUTY									(PWM_PERIOD_MAX * 0.95)				//PWM95%占空比
#define PWM_98_DUTY									(PWM_PERIOD_MAX * 0.98)				//PWM98%占空比

#define MIN_PWM_DUTY								(PWM_5_DUTY)						//PWM最小占空比 5%
#define MAX_PWM_DUTY								(PWM_98_DUTY)						//PWM最大占空比98%

#define MOTOR_HALL_MODE_MIN_DUTY						(PWM_20_DUTY)						//霍尔传感器模式下电机最小占空比
#define	MOTOR_SENSORLESS_MODE_MIN_DUTY				(PWM_20_DUTY)						//无传感器模式下电机最小占空比

#ifdef	MOTOR_HALL_MODE
#define MOTOR_START_MIN_DUTY						(MOTOR_HALL_MODE_MIN_DUTY)			//电机霍尔模式下最小启动占空比
#else
#define MOTOR_START_MIN_DUTY						(MOTOR_SENSORLESS_MODE_MIN_DUTY)	//电机无霍尔模式下最小启动占空比
#endif

#define SENSORLESS_PRE_POSITION_STEP_1_PWM_DUTY		(PWM_20_DUTY)						//转子预定位第1步占空比
#define SENSORLESS_PRE_POSITION_STEP_1_PWM_TIME		(1000)								//转子预定位第1步持续时间:20K->50us, 50ms
#define SENSORLESS_PRE_POSITION_STEP_2_PWM_DUTY		(PWM_20_DUTY)						//转子预定位第2步占空比
#define SENSORLESS_PRE_POSITION_STEP_2_PWM_TIME		(250)								//转子预定位第2步持续时间:20K->50us, 50ms


#define SENSORLESS_OPEN_LOOP_SYNC_ACC_MAX_TIME		(500)								//开环同步加速换相最大持续时间:20K->50us, 25ms
#define SENSORLESS_OPEN_LOOP_SYNC_ACC_MIN_TIME		(100)								//开环同步加速换相最小持续时间:20K->50us, 1ms
#define SENSORLESS_OPEN_LOOP_SYNC_ACC_TIME_DIFF_VAL	(5)								//开环同步加速换相时间差分参数

#define SENSORLESS_OPEN_LOOP_SYNC_ACC_MAX_DUTY		(MAX_PWM_DUTY * 0.5)				//开环同步加速阶段最大PWM占空比
#define SENSORLESS_OPEN_LOOP_SYNC_ACC_MIN_DUTY		(MAX_PWM_DUTY * 0.5 * 0.3)			//开环同步加速阶段最小PWM占空比
#define SENSORLESS_OPEN_LOOP_SYNC_ACC_DUTY_DIFF_VAL	(0.1)								//开环同步加速换相占空比差分参数

#define SENSORLESS_ZERO_FILTER_LONG					0x00000007							//过零点滤波长度

#define MOTOR_STALL_TIME_THRESHOULD					(500)										//堵转阈值		单位：ms
#define MOTOR_STALL_TIME_THRESHOULD1				(3000)									//堵转阈值1 	单位：ms

/*		
U、V、W三相线序定义方式为：逆时针CCW方向看为U->V->W
		U
		|
		|
   / \
  /   \
 V	   W 
*/
#define MOTOR_HALL_PHASE_CCW_U_V_W
/*		
U、V、W三相线序定义方式为：逆时针CCW方向看为U->W->V
		U
		|
		|
   / \
  /   \
 W	   V 
*/
#ifndef MOTOR_HALL_PHASE_CCW_U_V_W
#define MOTOR_HALL_PHASE_CCW_U_W_V
#endif

typedef struct{
	uint16_t pwm_duty;				//PWM占空比
	uint8_t motor_direction;	//电机运行方向
	uint8_t error_cnt;				//异常计数
	uint8_t error_sign;				//异常标记
	uint8_t error_type;				//异常类型
	uint8_t motor_sta;				//电机运行状态
	uint32_t calculate_speed;	//计算出的电机转速
	uint32_t motor_phase_time;//电机换相时间记录
}motor_ctrl_prama_t;

extern motor_ctrl_prama_t motor_ctrl_prama;

void motor_ctrl_init(void);
int motor_start(uint16_t start_pwm_duty,uint8_t motor_dir);
void motor_stop(void);
#endif 
