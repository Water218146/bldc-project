#ifndef __MOTOR_OPEN_LOOP_H_
#define __MOTOR_OPEN_LOOP_H_

#include "bsp_define.h"
#define MOTOR_CCW 0
#define MOTOR_CW  1
#define MOTOR_STOP 0
#define MOTOR_RUN  1

typedef struct
{
	uint8_t motor_cw_ccw;			//0 ccw 1 cw
	uint8_t motor_start_stop;	//0 stop 1 start
}motor_run_key_t;
extern motor_run_key_t motor_run_key;

void motor_open_ccw_operation(uint16_t duty);
void motor_open_cw_operation(uint16_t duty);
void motor_open_loop_task(void);
#endif
