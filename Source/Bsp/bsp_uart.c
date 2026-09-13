#include "bsp_uart.h"

com_irq_cb_t com_irq_cb = {NULL,NULL,NULL};

/**
  ******************************************************************************
  * @brief  UART关联RCC时钟配置
  * @param  com:端口号
  * @retval None.
  ******************************************************************************/
static void bsp_uart_rcc_config(uart_com_e com)
{
    switch (com)
    {
        case DEBUG_COM:
        /* 使能 debug gpio 时钟 */
            DEBUG_UART_TX_GPIO_CLK_CMD(DEBUG_UART_TX_GPIO_CLK, ENABLE);
            DEBUG_UART_RX_GPIO_CLK_CMD(DEBUG_UART_RX_GPIO_CLK, ENABLE);
        /* 使能 debug uart 时钟 */
            DEBUG_UART_CLK_CMD(DEBUG_UART_CLK, ENABLE);

            break;
        case HOST_COMPUTER_COM:
        /* 使能 host computer gpio 时钟 */
            HOST_COMPUTER_UART_TX_GPIO_CLK_CMD(HOST_COMPUTER_UART_TX_GPIO_CLK, ENABLE);
            HOST_COMPUTER_UART_RX_GPIO_CLK_CMD(HOST_COMPUTER_UART_RX_GPIO_CLK, ENABLE);
        /* 使能 host computer uart 时钟 */
            HOST_COMPUTER_UART_CLK_CMD(HOST_COMPUTER_UART_CLK, ENABLE);
            break;
        case RS485_COM:
        /* 使能 rs485 gpio 时钟 */
            RS485_UART_TX_GPIO_CLK_CMD(RS485_UART_TX_GPIO_CLK, ENABLE);
            RS485_UART_RX_GPIO_CLK_CMD(RS485_UART_RX_GPIO_CLK, ENABLE);
        /* 使能 rs485 EN GPIO 时钟 */
            RS485_EN_GPIO_CLK_CMD(RS485_EN_GPIO_CLK, ENABLE);
        /* 使能 rs485 uart 时钟 */
            RS485_UART_CLK_CMD(RS485_UART_CLK, ENABLE);
            break;
        default:
            break;
    }
}

/**
  ******************************************************************************
  * @brief  UART GPIO配置
  * @param  com:端口号
  * @retval None.
  ******************************************************************************/
 void bsp_uart_gpio_config(uart_com_e com)
 {
    GPIO_InitType GPIO_InitStructure = {0};
    GPIO_InitStruct(&GPIO_InitStructure);
    switch (com)
    {
        case DEBUG_COM:
            /* 配置 debug uart TX 引脚 */
            GPIO_InitStructure.Pin        = DEBUG_UART_TX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Alternate = DEBUG_UART_TX_GPIO_AF;
            GPIO_InitPeripheral(DEBUG_UART_TX_GPIO, &GPIO_InitStructure);

            /* 配置 debug uart RX 引脚 */
            GPIO_InitStructure.Pin        = DEBUG_UART_RX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
            GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
            GPIO_InitStructure.GPIO_Alternate = DEBUG_UART_RX_GPIO_AF;
            GPIO_InitPeripheral(DEBUG_UART_RX_GPIO, &GPIO_InitStructure);
            break;
        case HOST_COMPUTER_COM:
            /* 配置 host computer uart TX 引脚 */
            GPIO_InitStructure.Pin        = HOST_COMPUTER_UART_TX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Alternate = HOST_COMPUTER_UART_TX_GPIO_AF;
            GPIO_InitPeripheral(HOST_COMPUTER_UART_TX_GPIO, &GPIO_InitStructure);

            /* 配置 host computer uart RX 引脚 */
            GPIO_InitStructure.Pin        = HOST_COMPUTER_UART_RX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
            GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
            GPIO_InitStructure.GPIO_Alternate = HOST_COMPUTER_UART_RX_GPIO_AF;
            GPIO_InitPeripheral(HOST_COMPUTER_UART_RX_GPIO, &GPIO_InitStructure);
            break;
        case RS485_COM:
            /* 配置 rs485 uart TX 引脚 */
            GPIO_InitStructure.Pin        = RS485_UART_TX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Alternate = RS485_UART_TX_GPIO_AF;
            GPIO_InitPeripheral(RS485_UART_TX_GPIO, &GPIO_InitStructure);

            /* 配置 rs485 uart RX 引脚 */
            GPIO_InitStructure.Pin        = RS485_UART_RX_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
            GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
            GPIO_InitStructure.GPIO_Alternate = RS485_UART_RX_GPIO_AF;
            GPIO_InitPeripheral(RS485_UART_RX_GPIO, &GPIO_InitStructure);

            /* 配置 rs485 uart DE 引脚 */
            GPIO_InitStructure.Pin        = RS485_EN_PIN;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
            GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
            GPIO_InitPeripheral(RS485_EN_GPIO, &GPIO_InitStructure);
            GPIO_ResetBits(RS485_EN_GPIO, RS485_EN_PIN);

            RS485_COM_RECV_ENABLE();  		//初始设置为接受
            break;
    }
 }

/**
  ******************************************************************************
  * @brief  UART初始化
  * @param  com :端口号
  * @param  baud:设置波特率
  * @param  irq_cb:中断回调指针
  * @retval None.
  ******************************************************************************/
void bsp_uart_init(uart_com_e com, uint32_t baud, void (*irq_cb)(void))
{
	USART_InitType USART_InitStructure = {0};
	NVIC_InitType NVIC_InitStructure = {0};

	USART_StructInit(&USART_InitStructure);
	bsp_uart_rcc_config(com);
	bsp_uart_gpio_config(com);
	
	if(irq_cb == NULL)
	{
		while(1);
	}
	switch(com)
	{
		case DEBUG_COM:
			USART_InitStructure.BaudRate            = baud;
			USART_InitStructure.WordLength          = USART_WL_8B;
			USART_InitStructure.StopBits            = USART_STPB_1;
			USART_InitStructure.Parity              = USART_PE_NO;
			USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
			USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
		
			NVIC_InitStructure.NVIC_IRQChannel = DEBUG_UART_IRQ;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			USART_ConfigInt(DEBUG_UART, USART_INT_RXDNE, ENABLE);
			
			com_irq_cb.debug_com_cb = irq_cb;  //回调赋值
		
			/* Configure debug uart */
			USART_Init(DEBUG_UART, &USART_InitStructure);
			/* Enable the debug uart */
			USART_Enable(DEBUG_UART, ENABLE);
		break;
		case HOST_COMPUTER_COM:
			USART_InitStructure.BaudRate            = baud;
			USART_InitStructure.WordLength          = USART_WL_8B;
			USART_InitStructure.StopBits            = USART_STPB_1;
			USART_InitStructure.Parity              = USART_PE_NO;
			USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
			USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
		
			NVIC_InitStructure.NVIC_IRQChannel = HOST_COMPUTER_UART_IRQ;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			USART_ConfigInt(HOST_COMPUTER_UART, USART_INT_RXDNE,ENABLE);
			
			com_irq_cb.host_computer_com_cb = irq_cb;  //回调赋值
		
			/* Configure debug uart */
			USART_Init(HOST_COMPUTER_UART, &USART_InitStructure);
			/* Enable the debug uart */
			USART_Enable(HOST_COMPUTER_UART, ENABLE);
		break;
		case RS485_COM:
			USART_InitStructure.BaudRate            = baud;
			USART_InitStructure.WordLength          = USART_WL_8B;
			USART_InitStructure.StopBits            = USART_STPB_1;
			USART_InitStructure.Parity              = USART_PE_NO;
			USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
			USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
		
			NVIC_InitStructure.NVIC_IRQChannel = RS485_UART_IRQ;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			USART_ConfigInt(RS485_UART, USART_INT_RXDNE, ENABLE);
			
			com_irq_cb.rs485_com_cb = irq_cb;  //回调赋值
		
			/* Configure debug uart */
			USART_Init(RS485_UART, &USART_InitStructure);
			/* Enable the debug uart */
			USART_Enable(RS485_UART, ENABLE);
		break;
	}
}

/* 获取串口指针 */
static USART_Module *bsp_uart_get_instance(uart_com_e com)
{
	switch(com)
	{
		case DEBUG_COM:
			return DEBUG_UART;
		break;
		case HOST_COMPUTER_COM:
			return HOST_COMPUTER_UART;
		break;
		case RS485_COM:
			return RS485_UART;
		break;
		
		default:
			return NULL;
	}	
}

/* 单字节发送 */
static void bsp_uart_send_byte(USART_Module *uart,uint8_t data)
{
	if(uart == NULL)
	{
		while(1);
	}
	/* 等待发送寄存器空 */
	while(USART_GetFlagStatus(uart,USART_FLAG_TXDE) == RESET);
	
	USART_SendData(uart,data);
}

/* 多字节连续发送 */
static void bsp_uart_send_buffer(uart_com_e com,uint8_t *data,uint32_t len)
{
	USART_Module *uart = NULL;
	uint32_t i;
	if((data == NULL)||(len == 0))
	{
		while(1);
	}
	uart = bsp_uart_get_instance(com);
	if(uart == NULL)
	{
		while(1);
	}
	if(com == RS485_COM)
	{
		RS485_COM_SEND_ENABLE();
	}
	for(i = 0;i<len;i++)
	{
		bsp_uart_send_byte(uart,data[i]);
	}
	if(com == RS485_COM)
	{
		//等待发送完毕
		while(USART_GetFlagStatus(uart,USART_FLAG_TXC)==RESET)
		{
		}
		RS485_COM_RECV_ENABLE();
	}
}

void my_printf(uart_com_e com, const char *format, ...)
{
	char buffer[256]; // 临时存储格式化后的字符串
	va_list arg;      // 处理可变参数
	int len;          // 最终字符串长度
	uint32_t send_len;
	va_start(arg, format);
	// 安全地格式化字符串到 buffer
	len = vsnprintf(buffer, sizeof(buffer), format, arg);
	va_end(arg);
  
	if (len <= 0)
	{
		return;
	}
	/* vsnprintf 返回的是理论长度 可能大于实际buffer容量 */
	if((uint32_t)len >= sizeof(buffer))
	{
		send_len = sizeof(buffer) - 1;
	}
	else
	{
		send_len = (uint32_t)len;
	}
	 bsp_uart_send_buffer(com, (uint8_t *)buffer, send_len);
}



/* 串口task */
void debug_uart_task(void)
{
	if (debug_com_index == 0)
	{
		return;
	}
	if (n_tick - debug_com_tick >= DEBUG_TIMOUT_MS)
	{
		debug_com_tick = n_tick;//更新时间戳
		//start
		my_printf(DEBUG_COM,"debug_data:%s\r\n",debug_com_buffer);
		//end
		memset(debug_com_buffer, 0, sizeof(debug_com_buffer));
		debug_com_index = 0;
	}
}

/* 主机串口 task */
void host_computer_uart_task(void)
{
	if (host_computer_com_index == 0)
	{
		return;
	}
	if (n_tick - host_computer_com_tick >= HOST_TIMOUT_MS)
	{
		host_computer_com_tick = n_tick;//更新时间戳
		//start
		my_printf(HOST_COMPUTER_COM,"host_data:%s\r\n",host_computer_com_buffer);
		//end
		memset(host_computer_com_buffer, 0, sizeof(host_computer_com_buffer));
		host_computer_com_index = 0;
	}
}

/* RS485 串口 task */
void rs485_uart_task(void)
{
	if (rs485_com_index == 0)
	{
		return;
	}
	if (n_tick - rs485_com_tick >= RS485_TIMOUT_MS)
	{
		rs485_com_tick = n_tick;//更新时间戳
		//start
		my_printf(RS485_COM,"rs485_data:%s\r\n",rs485_com_buffer);
		//end
		memset(rs485_com_buffer, 0, sizeof(rs485_com_buffer));
		rs485_com_index = 0;
	}
}
