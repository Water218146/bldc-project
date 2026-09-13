#ifndef __BSP_UART_CB_H_
#define __BSP_UART_CB_H_

#include "bsp_define.h"
#define DEBUG_COM_BUF_SIZE 128
#define DEBUG_TIMOUT_MS 10
extern uint8_t debug_com_buffer[DEBUG_COM_BUF_SIZE];
extern volatile uint32_t debug_com_index;
extern volatile uint32_t debug_com_tick;
void debug_com_cb(void);

#define HOST_COM_BUF_SIZE 128
#define HOST_TIMOUT_MS 10
extern uint8_t host_computer_com_buffer[HOST_COM_BUF_SIZE];
extern volatile uint32_t host_computer_com_index;
extern volatile uint32_t host_computer_com_tick;
void host_computer_com_cb(void);

#define RS485_COM_BUF_SIZE 128
#define RS485_TIMOUT_MS 10
extern uint8_t rs485_com_buffer[RS485_COM_BUF_SIZE];
extern volatile uint32_t rs485_com_index;
extern volatile uint32_t rs485_com_tick;
void rs485_com_cb(void);


#endif
