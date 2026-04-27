/**
 * @file kt_port_uart_mock.c
 * @author kento
 * @brief Mock平台 UART模拟实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_MOCK

#include <stdio.h>
#include <string.h>

/**
 * @brief 初始化UART（模拟）
 */
kt_status_t kt_port_uart_init(void* uart_handle, kt_uint32_t baudrate,
                              kt_uart_data_t data_bits, kt_uart_stop_t stop_bits,
                              kt_uart_parity_t parity)
{
    printf("[MOCK] UART init: handle=%p, baudrate=%lu, data=%d, stop=%d, parity=%d\n", 
           uart_handle, baudrate, data_bits, stop_bits, parity);
    return KT_OK;
}

/**
 * @brief 通过UART发送数据（模拟）
 */
kt_int32_t kt_port_uart_send(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    printf("[MOCK] UART send (%d bytes): ", length);
    for (kt_uint16_t i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
    
    /* 尝试打印为ASCII字符串 */
    printf("[MOCK] UART send (ASCII): ");
    for (kt_uint16_t i = 0; i < length; i++)
    {
        if (data[i] >= 32 && data[i] <= 126)
        {
            printf("%c", data[i]);
        }
        else
        {
            printf(".");
        }
    }
    printf("\n");
    
    return (kt_int32_t)length;
}

/**
 * @brief 通过UART接收数据（模拟）
 */
kt_int32_t kt_port_uart_receive(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    printf("[MOCK] UART receive: waiting for %d bytes...\n", length);
    /* 模拟接收，返回0表示无数据 */
    return 0;
}

/**
 * @brief 通过UART发送数据（非阻塞，模拟）
 */
kt_status_t kt_port_uart_send_nb(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    return kt_port_uart_send(uart_handle, data, length);
}

/**
 * @brief 通过UART接收数据（非阻塞，模拟）
 */
kt_int32_t kt_port_uart_receive_nb(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    /* 模拟非阻塞接收，返回0表示无数据 */
    return 0;
}

#endif /* USE_KT_PLATFORM_MOCK */