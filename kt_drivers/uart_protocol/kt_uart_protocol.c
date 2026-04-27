/**
 * @file kt_uart_protocol.c
 * @author kento
 * @brief UART通信协议实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_uart_protocol.h"
#include "kt_port.h"

#if KT_DRIVER_UART_PROTOCOL_ENABLE

/**
 * @brief 初始化UART协议
 */
kt_status_t kt_uart_protocol_init(kt_uart_protocol_t* proto, void* uart_handle,
                                  kt_uint8_t* rx_buffer, kt_uint16_t rx_buf_size)
{
    if (proto == NULL || uart_handle == NULL || rx_buffer == NULL || rx_buf_size == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    proto->uart_handle = uart_handle;
    proto->state = KT_UART_PROTO_STATE_IDLE;
    proto->rx_cmd = 0;
    proto->rx_len = 0;
    proto->rx_pos = 0;
    proto->rx_buffer = rx_buffer;
    proto->rx_buf_size = rx_buf_size;
    proto->rx_callback = NULL;
    proto->callback_arg = NULL;
    proto->error_count = 0;
    
    return KT_OK;
}

/**
 * @brief 设置接收回调
 */
kt_status_t kt_uart_protocol_set_callback(kt_uart_protocol_t* proto, 
                                          kt_uart_rx_callback_t callback, void* arg)
{
    if (proto == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    proto->rx_callback = callback;
    proto->callback_arg = arg;
    
    return KT_OK;
}

/**
 * @brief 处理单个字节
 */
static void uart_protocol_process_byte(kt_uart_protocol_t* proto, kt_uint8_t byte)
{
    switch (proto->state)
    {
        case KT_UART_PROTO_STATE_IDLE:
            if (byte == KT_UART_FRAME_HEAD_1)
            {
                proto->state = KT_UART_PROTO_STATE_HEAD1;
            }
            break;
            
        case KT_UART_PROTO_STATE_HEAD1:
            if (byte == KT_UART_FRAME_HEAD_2)
            {
                proto->state = KT_UART_PROTO_STATE_HEAD2;
            }
            else if (byte != KT_UART_FRAME_HEAD_1)
            {
                proto->state = KT_UART_PROTO_STATE_IDLE;
            }
            break;
            
        case KT_UART_PROTO_STATE_HEAD2:
            proto->rx_cmd = byte;
            proto->state = KT_UART_PROTO_STATE_CMD;
            break;
            
        case KT_UART_PROTO_STATE_CMD:
            /* 接收长度字段（高字节） */
            proto->rx_len = (kt_uint16_t)byte << 8;
            proto->state = KT_UART_PROTO_STATE_LEN;
            break;
            
        case KT_UART_PROTO_STATE_LEN:
            /* 接收长度字段（低字节） */
            proto->rx_len |= byte;
            
            /* 检查数据长度是否超过缓冲区 */
            if (proto->rx_len > proto->rx_buf_size)
            {
                proto->error_count++;
                proto->state = KT_UART_PROTO_STATE_IDLE;
                break;
            }
            
            proto->rx_pos = 0;
            if (proto->rx_len == 0)
            {
                proto->state = KT_UART_PROTO_STATE_CRC;
            }
            else
            {
                proto->state = KT_UART_PROTO_STATE_DATA;
            }
            break;
            
        case KT_UART_PROTO_STATE_DATA:
            proto->rx_buffer[proto->rx_pos++] = byte;
            if (proto->rx_pos >= proto->rx_len)
            {
                proto->state = KT_UART_PROTO_STATE_CRC;
            }
            break;
            
        case KT_UART_PROTO_STATE_CRC:
            /* CRC校验在完整实现中处理 */
            /* 这里简化处理，直接调用回调 */
            if (proto->rx_callback != NULL)
            {
                proto->rx_callback(proto->callback_arg, proto->rx_cmd, 
                                   proto->rx_buffer, proto->rx_len);
            }
            proto->state = KT_UART_PROTO_STATE_IDLE;
            break;
            
        default:
            proto->state = KT_UART_PROTO_STATE_IDLE;
            break;
    }
}

/**
 * @brief 处理接收到的数据
 */
void kt_uart_protocol_receive(kt_uart_protocol_t* proto, const kt_uint8_t* data, kt_uint16_t len)
{
    kt_uint16_t i;
    
    if (proto == NULL || data == NULL || len == 0)
    {
        return;
    }
    
    for (i = 0; i < len; i++)
    {
        uart_protocol_process_byte(proto, data[i]);
    }
}

/**
 * @brief 发送数据帧
 */
kt_int32_t kt_uart_protocol_send(kt_uart_protocol_t* proto, kt_uint8_t cmd, 
                                 const kt_uint8_t* data, kt_uint16_t len)
{
    kt_uint16_t total_len;
    kt_uint16_t pos = 0;
    
    if (proto == NULL || len > sizeof(proto->tx_buffer) - KT_UART_MIN_FRAME_LEN)
    {
        return 0;
    }
    
    /* 构建帧 */
    proto->tx_buffer[pos++] = KT_UART_FRAME_HEAD_1;
    proto->tx_buffer[pos++] = KT_UART_FRAME_HEAD_2;
    proto->tx_buffer[pos++] = cmd;
    proto->tx_buffer[pos++] = (kt_uint8_t)(len >> 8);  /* 高字节 */
    proto->tx_buffer[pos++] = (kt_uint8_t)(len & 0xFF); /* 低字节 */
    
    if (len > 0 && data != NULL)
    {
        for (kt_uint16_t i = 0; i < len; i++)
        {
            proto->tx_buffer[pos++] = data[i];
        }
    }
    
    /* CRC校验（简化版本，实际需要计算CRC16） */
    proto->tx_buffer[pos++] = 0x00;  /* CRC高字节（占位） */
    proto->tx_buffer[pos++] = 0x00;  /* CRC低字节（占位） */
    
    total_len = pos;
    
    /* 发送数据 */
    return kt_port_uart_send(proto->uart_handle, proto->tx_buffer, total_len);
}

/**
 * @brief 重置协议状态
 */
void kt_uart_protocol_reset(kt_uart_protocol_t* proto)
{
    if (proto == NULL)
    {
        return;
    }
    
    proto->state = KT_UART_PROTO_STATE_IDLE;
    proto->rx_cmd = 0;
    proto->rx_len = 0;
    proto->rx_pos = 0;
    proto->error_count = 0;
}

/**
 * @brief 获取错误计数
 */
kt_uint32_t kt_uart_protocol_get_error_count(kt_uart_protocol_t* proto)
{
    if (proto == NULL)
    {
        return 0;
    }
    
    return proto->error_count;
}

#endif /* KT_DRIVER_UART_PROTOCOL_ENABLE */