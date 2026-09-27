#ifndef __MOTOR_SENSOR_H_
#define __MOTOR_SENSOR_H_

#include "bsp_define.h"
typedef struct{
	uint8_t u_value;
	uint8_t v_value;
	uint8_t w_value;
	uint8_t value;
	uint8_t level_sign;			//电平跳变标记
	uint32_t rising_time;		//上升沿时间
	uint32_t falling_time;  //下降沿时间
	uint32_t sp_filter;			//速度滤波器
}hall_value_t;

extern hall_value_t hall_value;

/* 有感算法 */
void motor_get_hall_value(void);
void motor_sensor_mode_phase(void);



#endif