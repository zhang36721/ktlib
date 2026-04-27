/**
 * @file kt_port_uart_esp32.c
 * @author kento
 * @brief ESP32 UART适配实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_ESP32

#include "driver/uart.h"

/**
 * @brief 初始化UART
 */
kt_status_t kt_port_uart_init(void* uart_handle, kt_uint32_t baudrate,
                              kt_uart_data_t data_bits, kt_uart_stop_t stop_bits,
                              kt_uart_parity_t parity)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    uart_config_t uart_config = {0};
    
    /* 检查参数 */
    if (uart_num < UART_NUM_0 || uart_num > UART_NUM_2)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 配置UART参数 */
    uart_config.baud_rate = baudrate;
    
    /* 数据位 */
    switch (data_bits)
    {
        case KT_UART_DATA_5:
            uart_config.data_bits = UART_DATA_5_BITS;
            break;
        case KT_UART_DATA_6:
            uart_config.data_bits = UART_DATA_6_BITS;
            break;
        case KT_UART_DATA_7:
            uart_config.data_bits = UART_DATA_7_BITS;
            break;
        case KT_UART_DATA_8:
        default:
            uart_config.data_bits = UART_DATA_8_BITS;
            break;
    }
    
    /* 停止位 */
    switch (stop_bits)
    {
        case KT_UART_STOP_1:
            uart_config.stop_bits = UART_STOP_BITS_1;
            break;
        case KT_UART_STOP_2:
            uart_config.stop_bits = UART_STOP_BITS_2;
            break;
        default:
            uart_config.stop_bits = UART_STOP_BITS_1;
            break;
    }
    
    /* 校验位 */
    switch (parity)
    {
        case KT_UART_PARITY_NONE:
            uart_config.parity = UART_PARITY_DISABLE;
            break;
        case KT_UART_PARITY_EVEN:
            uart_config.parity = UART_PARITY_EVEN;
            break;
        case KT_UART_PARITY_ODD:
            uart_config.parity = UART_PARITY_ODD;
            break;
        default:
            uart_config.parity = UART_PARITY_DISABLE;
            break;
    }
    
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    
    /* 配置UART参数 */
    uart_param_config(uart_num, &uart_config);
    
    /* 安装UART驱动 */
    uart_driver_install(uart_num, 256, 0, 0, NULL, 0);
    
    return KT_OK;
}

/**
 * @brief 通过UART发送数据（阻塞）
 */
kt_int32_t kt_port_uart_send(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    
    if (uart_num < UART_NUM_0 || uart_num > UART_NUM_2)
    {
        return 0;
    }
    
    return (kt_int32_t)uart_write_bytes(uart_num, (const char*)data, length);
}

/**
 * @brief 通过UART接收数据（阻塞）
 */
kt_int32_t kt_port_uart_receive(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    
    if (uart_num < UART_NUM_0 || uart_num > UART_NUM_2)
    {
        return 0;
    }
    
    return (kt_int32_t)uart_read_bytes(uart_num, data, length, portMAX_DELAY);
}

/**
 * @brief 通过UART发送数据（非阻塞）
 */
kt_status_t kt_port_uart_send_nb(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    
    if (uart_num < UART_NUM_0 || uart_num > UART_NUM_2)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* ESP32的uart_write_bytes本身就是非阻塞的 */
    uart_write_bytes(uart_num, (const char*)data, length);
    
    return KT_OK;
}

/**
 * @brief 通过UART接收数据（非阻塞）
 */
kt_int32_t kt_port_uart_receive_nb(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    
    if (uart_num < UART_NUM_0 || uart_num > UART_NUM_2)
    {
        return 0;
    }
    
    /* 使用0超时实现非阻塞接收 */
    return (kt_int32_t)uart_read_bytes(uart_num, data, length, 0);
}

#endif /* USE_KT_PLATFORM_ESP32 */