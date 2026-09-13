#include "bsp_define.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置中断分组为4组抢占优先级，0组响应优先级
	bsp_systick_init(); 	//初始化系统滴答定时器
	bsp_uart_init(DEBUG_COM,115200,debug_com_cb);//Debug串口初始化
	my_printf(DEBUG_COM,"debug_uart:ok!\r\n");
	bsp_uart_init(HOST_COMPUTER_COM,115200,host_computer_com_cb);//主机调试接口初始化	
	my_printf(HOST_COMPUTER_COM,"host_uart:ok!\r\n");	
	bsp_uart_init(RS485_COM,115200,rs485_com_cb);//RS485初始化
	my_printf(RS485_COM,"rs485_uart:ok!\r\n");	
	bsp_key_init();	//按键初始化
	my_printf(DEBUG_COM,"key_init:ok!\r\n");
	bsp_led_init();	//LED初始化
	my_printf(DEBUG_COM,"led_init:ok!\r\n");
	bsp_dac_init(300);		//241mV电压	 3.3 / 4096 = 0.0008V = 0.8 mv * 300 = 241 mv
	my_printf(DEBUG_COM,"dac_init:ok!,adc_value:241!!\r\n");
	bsp_opa1_init();
	my_printf(DEBUG_COM,"opa1_init:ok!\r\n");
	bsp_adc_init(bsp_adc_irq_cb,motor_sensorless_mode_phase);
	my_printf(DEBUG_COM,"adc_init:ok!\r\n");
	bsp_pwm_init(bsp_pwm_brake_irq_cb,bsp_pwm_irq_cb);
	my_printf(DEBUG_COM,"pwm_init:ok!\r\n");	
	bsp_pwm_duty_set(1349);
	
	scheduler_init(); 	//初始化调度器
	my_printf(DEBUG_COM,"scheduler_init:ok!\r\n");	
	while(1)
	{
		scheduler_run(); 	//运行调度器
	}
	
}


