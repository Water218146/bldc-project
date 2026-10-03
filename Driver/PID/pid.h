#ifndef __PID_H_
#define __PID_H_


typedef struct pid_ctrl_t
{
    float kp;                               //比例系数
    float ki;                               //积分系数
    float kd;                               //微分系数

    float target_value;                     //目标值
    float current_value;                    //当前值
    float error;                            //当前误差
    float last_error;                       //上次误差
    float prav_error;                       //上上次误差

    /* 误差带:检测到实际控制值进入到误差带范围内之后 则认为已经达到目标值 */
    float error_p_band;                     //正误差带
    float error_n_band;                     //负误差带

    float i_acc_value;                      //积分项累加值
    float i_separate_p_threshold_value;     //积分分离正阈值
    float i_separate_n_threshold_value;     //积分分离负阈值
    float i_windup_p_threshold_value;       //积分限幅正阈值
    float i_windup_n_threshold_value;       //积分限幅负阈值

    float p_value;                          //比例项计算结果
    float i_value;                          //积分项计算结果
    float d_value;                          //微分项计算结果

    float uk_max_value;                     //输出最大值
    float uk_min_value;                     //输出最小值
    float uk_value;                         //计算结果输出值
    
    void (*pid_funtion)(struct pid_ctrl_t *pid_ctrl);//Pid控制对象
}pid_ctrl_tt;

void position_pid(struct pid_ctrl_t *pid_ctrl);     //位置式PID
void incremental_pid(struct pid_ctrl_t *pid_ctrl);  //增量式PID

#endif