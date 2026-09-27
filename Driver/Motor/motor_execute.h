#ifndef __MOTOR_EXECUTE_H_
#define __MOTOR_EXECUTE_H_

#include "bsp_define.h"

typedef enum
{
	EXECUTE_IDLE = 0,							//执行空闲状态
	EXECUTE_MOTOR_START,					//电机启动
	EXECUTE_MOTOR_EXECUTE,				//电机执行
	EXECUTE_MOTOR_STOP,						//电机停止
}motor_execute_state_machine_e;


void motor_open_speed(void);			//开环速度计算函数
void motor_execute_task(void);			//电机状态机任务
void motor_error_check(void);			//电机错误检测函数
#endif
