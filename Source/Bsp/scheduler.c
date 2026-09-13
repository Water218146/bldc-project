#include "scheduler.h"

typedef struct
{
    void (*task_func)(void); // 任务函数指针
    uint32_t rate_ms;         // 任务周期，单位为毫秒
    uint32_t last_ms;  // 上次运行时间，单位为毫秒
}task_t;

static task_t scheduler_task[] = {
    {debug_uart_task, 5, 0}, // debug_uart
    {host_computer_uart_task, 5, 0}, // host_computer_uart
    {rs485_uart_task, 5, 0}, // rs485_uart
		{key_task , 10 ,0},
		{led_task , 1	 ,0},
		{adc_calculate_task,50,0},
};

static uint8_t task_num; // 全局变量，用于存储任务数量

void scheduler_init(void)
{
    task_num = sizeof(scheduler_task) / sizeof(task_t); // 计算任务数组的元素个数，并将结果存储在 task_num 中
}

void scheduler_run(void)
{
    for (uint8_t i = 0; i < task_num; i++)
    {
        uint32_t now_time = bsp_systick_get_tick(); // 获取当前的系统时间（毫秒）
        if((uint32_t)(now_time - scheduler_task[i].last_ms) >= scheduler_task[i].rate_ms) // 检查当前时间是否达到任务的执行时间
        {
            scheduler_task[i].last_ms = now_time; // 更新任务的上次运行时间为当前时间
            scheduler_task[i].task_func(); // 执行任务函数
        }   

        // 检查当前时间是否达到任务的执行时间

    }
}
