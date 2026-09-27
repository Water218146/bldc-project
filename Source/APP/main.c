#include "bsp_define.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);													//设置中断分组为4组抢占优先级，0组响应优先级
	bsp_systick_init(); 																										//初始化系统滴答定时器
	bsp_uart_init(DEBUG_COM,115200,debug_com_cb);														//Debug串口初始化
	my_printf(DEBUG_COM,"debug_uart:ok!\r\n");
	bsp_uart_init(HOST_COMPUTER_COM,115200,host_computer_com_cb);						//主机调试接口初始化	
	my_printf(HOST_COMPUTER_COM,"host_uart:ok!\r\n");	
	bsp_uart_init(RS485_COM,115200,rs485_com_cb);														//RS485初始化
	my_printf(RS485_COM,"rs485_uart:ok!\r\n");	
	bsp_io_init();																													//IO初始化
	my_printf(DEBUG_COM,"io_init:ok!");
//	ADC_TEST_IO_HIGH();
	bsp_key_init();																													//按键初始化
	my_printf(DEBUG_COM,"key_init:ok!\r\n");
	bsp_led_init();																													//LED初始化
	my_printf(DEBUG_COM,"led_init:ok!\r\n");
	bsp_dac_init(397);			//320mv电压   3.3 / 4095 * 397 = 0.32v ->对应2mΩ电流采样电阻 在5A极限电流下 经过内部运放32倍放大后的电压值 用于过流保护
	my_printf(DEBUG_COM,"dac_init:ok!,adc_value:241!!\r\n");
	bsp_opa1_init();																												//内部运放初始化
	my_printf(DEBUG_COM,"opa1_init:ok!\r\n");
	bsp_adc_init(bsp_adc_irq_cb,motor_sensorless_mode_phase);								//adc初始化
	my_printf(DEBUG_COM,"adc_init:ok!\r\n");
	bsp_pwm_init(bsp_pwm_brake_irq_cb,bsp_pwm_irq_cb);											//pwm初始化
	my_printf(DEBUG_COM,"pwm_init:ok!\r\n");	
//	bsp_pwm_duty_set(1349);
	bsp_hall_init(hall_uvw_irq_cb,motor_sensor_mode_phase);									//hall传感器读取初始化
	my_printf(DEBUG_COM,"hall_init:ok!\r\n");
	motor_ctrl_init();																											//电机状态初始化
	my_printf(DEBUG_COM,"motor_ctrl_init:ok!\r\n");
	bsp_timer8_init(bsp_timer8_irq_cb);																			//timer8初始化
	my_printf(DEBUG_COM,"timer8_init:ok!");
	
	
	scheduler_init(); 																											//调度器初始化
	my_printf(DEBUG_COM,"scheduler_init:ok!\r\n");	

	while(1)
	{
		scheduler_run(); 	//运行调度器
	}
	
}


