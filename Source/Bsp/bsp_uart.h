#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "bsp_define.h"

/************************************************************************/
#define DEBUG_UART            					UART4
#define DEBUG_UART_CLK        					RCC_APB2_PERIPH_UART4

#define DEBUG_UART_TX_GPIO       				GPIOB
#define DEBUG_UART_TX_PIN      					GPIO_PIN_0
#define DEBUG_UART_TX_GPIO_CLK   				RCC_APB2_PERIPH_GPIOB
#define DEBUG_UART_TX_GPIO_AF 					GPIO_AF6_UART4

#define DEBUG_UART_RX_GPIO       				GPIOB
#define DEBUG_UART_RX_PIN      					GPIO_PIN_1
#define DEBUG_UART_RX_GPIO_CLK   				RCC_APB2_PERIPH_GPIOB
#define DEBUG_UART_RX_GPIO_AF 					GPIO_AF6_UART4


#define DEBUG_UART_TX_GPIO_CLK_CMD   			RCC_EnableAPB2PeriphClk
#define DEBUG_UART_RX_GPIO_CLK_CMD   			RCC_EnableAPB2PeriphClk
#define DEBUG_UART_CLK_CMD  					RCC_EnableAPB2PeriphClk

#define DEBUG_UART_IRQ      					UART4_IRQn


/************************************************************************/
#define HOST_COMPUTER_UART            			UART5
#define HOST_COMPUTER_UART_CLK        			RCC_APB2_PERIPH_UART5

#define HOST_COMPUTER_UART_TX_GPIO       		GPIOC
#define HOST_COMPUTER_UART_TX_PIN      			GPIO_PIN_12
#define HOST_COMPUTER_UART_TX_GPIO_CLK   		RCC_APB2_PERIPH_GPIOC
#define HOST_COMPUTER_UART_TX_GPIO_AF 			GPIO_AF6_UART5

#define HOST_COMPUTER_UART_RX_GPIO       		GPIOD
#define HOST_COMPUTER_UART_RX_PIN      			GPIO_PIN_2
#define HOST_COMPUTER_UART_RX_GPIO_CLK   		RCC_APB2_PERIPH_GPIOD
#define HOST_COMPUTER_UART_RX_GPIO_AF 			GPIO_AF6_UART5


#define HOST_COMPUTER_UART_TX_GPIO_CLK_CMD   	RCC_EnableAPB2PeriphClk
#define HOST_COMPUTER_UART_RX_GPIO_CLK_CMD   	RCC_EnableAPB2PeriphClk
#define HOST_COMPUTER_UART_CLK_CMD  			RCC_EnableAPB2PeriphClk

#define HOST_COMPUTER_UART_IRQ					UART5_IRQn

/************************************************************************/
#define RS485_UART            					USART3
#define RS485_UART_CLK        					RCC_APB1_PERIPH_USART3  

#define RS485_UART_TX_GPIO       				GPIOB
#define RS485_UART_TX_PIN      					GPIO_PIN_10
#define RS485_UART_TX_GPIO_CLK   				RCC_APB2_PERIPH_GPIOB
#define RS485_UART_TX_GPIO_AF 					GPIO_AF0_USART3

#define RS485_UART_RX_GPIO       				GPIOB
#define RS485_UART_RX_PIN      					GPIO_PIN_11
#define RS485_UART_RX_GPIO_CLK   				RCC_APB2_PERIPH_GPIOB
#define RS485_UART_RX_GPIO_AF 					GPIO_AF5_USART3

#define RS485_EN_GPIO       					GPIOC
#define RS485_EN_PIN      						GPIO_PIN_8
#define RS485_EN_GPIO_CLK   					RCC_APB2_PERIPH_GPIOC

#define RS485_UART_TX_GPIO_CLK_CMD   			RCC_EnableAPB2PeriphClk
#define RS485_UART_RX_GPIO_CLK_CMD   			RCC_EnableAPB2PeriphClk
#define RS485_EN_GPIO_CLK_CMD   				RCC_EnableAPB2PeriphClk
#define RS485_UART_CLK_CMD  					RCC_EnableAPB1PeriphClk

#define RS485_UART_IRQ							USART3_IRQn


#define RS485_COM_SEND_ENABLE()					GPIO_ResetBits(RS485_EN_GPIO, RS485_EN_PIN)     //RS485端口发送使能                
#define RS485_COM_RECV_ENABLE()					GPIO_SetBits(RS485_EN_GPIO, RS485_EN_PIN)       //RS485端口接收使能                


typedef enum
{   
    DEBUG_COM = 0,
    HOST_COMPUTER_COM,
    RS485_COM,
}uart_com_e;

typedef struct
{  
    void (*debug_com_cb)(void);
    void (*host_computer_com_cb)(void);
    void (*rs485_com_cb)(void);
}com_irq_cb_t;

extern com_irq_cb_t com_irq_cb;

void bsp_uart_init(uart_com_e com, uint32_t baud, void (*irq_cb)(void));//串口初始化
void my_printf(uart_com_e com, const char *format, ...);//串口打印函数
void debug_uart_task(void);
void host_computer_uart_task(void);
void rs485_uart_task(void);

#endif
