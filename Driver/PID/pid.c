/**
  ******************************************************************************
  * @file    pid.c
  * @author  Water
  * @version V1.0
  * @date    2026-10-01
  * @brief   电机控制
  ******************************************************************************/
#include "pid.h"


/**
  ******************************************************************************
  * @brief  位置式PID
  * @param  pid_ctrl：pid控制结构体
  * @retval None.
  ******************************************************************************/
void position_pid(struct pid_ctrl_t *pid_ctrl)
{
    float uk = 0;   //输出值临时变量 
    /* 计算当前实时误差 */
    pid_ctrl ->error = pid_ctrl ->target_value - pid_ctrl -> current_value;  

    /* 在误差带范围内 直接将误差设置为0 */
    if((pid_ctrl -> error > pid_ctrl ->error_n_band)&&(pid_ctrl -> error < pid_ctrl -> error_p_band))
    {
        pid_ctrl -> error = 0;
    }

    /* 比例项计算 */
    pid_ctrl->p_value = pid_ctrl->kp * pid_ctrl->error;
#if 0
    float ki_k = 0; //积分分离系数

    /* 积分分离 */
    if((pid_ctrl->error >= pid_ctrl->i_separate_n_threshold_value)&&(pid_ctrl->error <= pid_ctrl->i_separate_p_threshold_value))
    {
        ki_k = 1;
        //积分值累加
        pid_ctrl->i_acc_value += pid_ctrl->error;
        //积分限幅
        if(pid_ctrl->i_acc_value < pid_ctrl->i_windup_n_threshold_value)//积分累加值小于下限阈值
        {
            pid_ctrl->i_acc_value = pid_ctrl->i_windup_n_threshold_value;
        }
        if(pid_ctrl->i_acc_value > pid_ctrl->i_windup_p_threshold_value)//积分累加值大于积分上限阈值
        {
            pid_ctrl->i_acc_value = pid_ctrl->i_windup_p_threshold_value;
        }       
    }
    /* 超过积分分离阈值：积分项累加值直接清零 */
    else
    {
        pid_ctrl -> i_acc_value = 0;
        ki_k = 0;
    }
    /* 积分项计算 */
    pid_ctrl->i_value = pid_ctrl->ki * pid_ctrl->i_acc_value;
#else
    /* 积分项累加 */
    pid_ctrl->i_acc_value += pid_ctrl->error;   //不断累加误差值
    /* 积分项计算 */
    pid_ctrl->i_value = pid_ctrl->ki * pid_ctrl->i_acc_value;
#endif

    /* 微分项计算 */
    pid_ctrl->d_value = pid_ctrl->kd * (pid_ctrl->error - pid_ctrl->last_error);

    /* 计算输出值 */
    uk = pid_ctrl->p_value + pid_ctrl->i_value + pid_ctrl->d_value;

    //更新误差值
    pid_ctrl->last_error = pid_ctrl->error;

    pid_ctrl->uk_value = uk;   //保存输出值

    /* 对输出值进行限幅 */
    if(pid_ctrl->uk_value < pid_ctrl->uk_min_value)
        pid_ctrl->uk_value = pid_ctrl->uk_min_value;
    if(pid_ctrl->uk_value > pid_ctrl->uk_max_value)
        pid_ctrl->uk_value = pid_ctrl->uk_max_value;
}

/**
  ******************************************************************************
  * @brief  增量式PID
  * @param  pid_ctrl：pid控制结构体
  * @retval None.
  ******************************************************************************/
void incremental_pid(struct pid_ctrl_t *pid_ctrl)
{
    float uk = 0;   //输出值
    /* 计算当前误差值 */
    pid_ctrl->error = pid_ctrl->target_value - pid_ctrl->current_value;

    /* 计算比例项 */
    pid_ctrl->p_value = pid_ctrl->kp * (pid_ctrl->error - pid_ctrl->last_error);

    /* 计算积分项 */
    pid_ctrl->i_value = pid_ctrl->ki * pid_ctrl->error;

    /* 计算微分项 */
    pid_ctrl->d_value = pid_ctrl->kd * (pid_ctrl->error - 2*pid_ctrl->last_error +pid_ctrl->prav_error);

    /* 计算输出值 */
    uk = pid_ctrl->p_value + pid_ctrl->i_value + pid_ctrl->d_value;

    /* 更新历史误差 */
    pid_ctrl->prav_error = pid_ctrl->last_error;   //先更新上上次误差
    pid_ctrl->last_error = pid_ctrl->error;        //在更新当前误差

    pid_ctrl->uk_value += uk;   //保存输出值

    /* 对输出值进行限幅 */
    if(pid_ctrl->uk_value < pid_ctrl->uk_min_value)
        pid_ctrl->uk_value = pid_ctrl->uk_min_value;
    if(pid_ctrl->uk_value > pid_ctrl->uk_max_value)
        pid_ctrl->uk_value = pid_ctrl->uk_max_value;   

}