#include "bsp_uart_cb.h"

uint8_t debug_com_buffer[DEBUG_COM_BUF_SIZE] = {0};
volatile uint32_t debug_com_index = 0;
volatile uint32_t debug_com_tick = 0;

uint8_t host_computer_com_buffer[HOST_COM_BUF_SIZE] = {0};
volatile uint32_t host_computer_com_index = 0;
volatile uint32_t host_computer_com_tick = 0;

uint8_t rs485_com_buffer[RS485_COM_BUF_SIZE] = {0};
volatile uint32_t rs485_com_index = 0;
volatile uint32_t rs485_com_tick = 0;

void debug_com_cb(void)
{
    uint8_t data;

    if (USART_GetIntStatus(DEBUG_UART, USART_INT_RXDNE) != RESET) // 有字节进入中断
    {
        debug_com_tick = n_tick; // 更新时间戳	
        // 获取该字节数据
        data = (uint8_t)USART_ReceiveData(DEBUG_UART);

        if (debug_com_index >= sizeof(debug_com_buffer))
        {
            memset(debug_com_buffer, 0, sizeof(debug_com_buffer));
            debug_com_index = 0;
        }
        debug_com_buffer[debug_com_index++] = data; // 把数据放入 buffer
    }
}

void host_computer_com_cb(void)
{
    uint8_t data;

    if (USART_GetIntStatus(HOST_COMPUTER_UART, USART_INT_RXDNE) != RESET) // 有字节进入中断
    {
        host_computer_com_tick = n_tick; // 更新时间戳
        // 获取该字节数据
        data = (uint8_t)USART_ReceiveData(HOST_COMPUTER_UART);

        if (host_computer_com_index >= sizeof(host_computer_com_buffer))
        {
            memset(host_computer_com_buffer, 0, sizeof(host_computer_com_buffer));
            host_computer_com_index = 0;
        }
        host_computer_com_buffer[host_computer_com_index++] = data; // 把数据放入 buffer
    }
}

void rs485_com_cb(void)
{
    uint8_t data;

    if (USART_GetIntStatus(RS485_UART, USART_INT_RXDNE) != RESET) // 有字节进入中断
    {
        rs485_com_tick = n_tick; // 更新时间戳
        // 获取该字节数据
        data = (uint8_t)USART_ReceiveData(RS485_UART);

        if (rs485_com_index >= sizeof(rs485_com_buffer))
        {
            memset(rs485_com_buffer, 0, sizeof(rs485_com_buffer));
            rs485_com_index = 0;
        }
        rs485_com_buffer[rs485_com_index++] = data; // 把数据放入 buffer
    }
}
