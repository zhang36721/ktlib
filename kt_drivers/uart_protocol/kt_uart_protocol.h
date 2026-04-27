/**
 * @file kt_uart_protocol.h
 * @author kento
 * @brief UART通信协议
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_UART_PROTOCOL_H
#define KT_UART_PROTOCOL_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_DRIVER_UART_PROTOCOL_ENABLE

/*============================================================================
 *                         协议帧格式
 *============================================================================*/

/*
 * 帧格式：
 * +------+-------+-------+--------+-------+
 * | HEAD |  CMD  |  LEN  |  DATA  | CHECK |
 * +------+-------+-------+--------+-------+
 * | 1B   |  1B   |  2B   |  N B   |  2B   |
 * +------+-------+-------+--------+-------+
 *
 * HEAD: 帧头 (0xAA 0x55)
 * CMD:  命令字
 * LEN:  数据长度（大端序）
 * DATA: 数据内容
 * CHECK: CRC16校验
 */

/** 帧头 */
#define KT_UART_FRAME_HEAD_1    0xAA
#define KT_UART_FRAME_HEAD_2    0x55

/** 帧头长度 */
#define KT_UART_HEAD_LEN        2

/** 命令字长度 */
#define KT_UART_CMD_LEN         1

/** 长度字段长度 */
#define KT_UART_LEN_LEN         2

/** CRC校验长度 */
#define KT_UART_CRC_LEN         2

/** 最小帧长度（不含数据） */
#define KT_UART_MIN_FRAME_LEN   (KT_UART_HEAD_LEN + KT_UART_CMD_LEN + KT_UART_LEN_LEN + KT_UART_CRC_LEN)

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief 协议状态
 */
typedef enum
{
    KT_UART_PROTO_STATE_IDLE = 0,     /**< 空闲 */
    KT_UART_PROTO_STATE_HEAD1,        /**< 收到帧头1 */
    KT_UART_PROTO_STATE_HEAD2,        /**< 收到帧头2 */
    KT_UART_PROTO_STATE_CMD,          /**< 收到命令 */
    KT_UART_PROTO_STATE_LEN,          /**< 收到长度 */
    KT_UART_PROTO_STATE_DATA,         /**< 收到数据 */
    KT_UART_PROTO_STATE_CRC           /**< 收到CRC */
} kt_uart_proto_state_t;

/**
 * @brief 数据接收回调函数类型
 */
typedef void (*kt_uart_rx_callback_t)(void* arg, kt_uint8_t cmd, const kt_uint8_t* data, kt_uint16_t len);

/**
 * @brief UART协议结构体
 */
typedef struct
{
    void*                       uart_handle;        /**< UART句柄 */
    kt_uart_proto_state_t       state;              /**< 协议状态 */
    kt_uint8_t                  rx_cmd;             /**< 当前接收的命令 */
    kt_uint16_t                 rx_len;             /**< 当前接收的数据长度 */
    kt_uint16_t                 rx_pos;             /**< 当前接收位置 */
    kt_uint8_t*                 rx_buffer;          /**< 接收缓冲区 */
    kt_uint16_t                 rx_buf_size;        /**< 接收缓冲区大小 */
    kt_uint8_t                  tx_buffer[64];      /**< 发送缓冲区 */
    kt_uart_rx_callback_t       rx_callback;        /**< 接收回调 */
    void*                       callback_arg;       /**< 回调参数 */
    kt_uint32_t                 error_count;        /**< 错误计数 */
} kt_uart_protocol_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化UART协议
 * @param proto 协议指针
 * @param uart_handle UART句柄
 * @param rx_buffer 接收缓冲区
 * @param rx_buf_size 接收缓冲区大小
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_uart_protocol_init(kt_uart_protocol_t* proto, void* uart_handle,
                                  kt_uint8_t* rx_buffer, kt_uint16_t rx_buf_size);

/**
 * @brief 设置接收回调
 * @param proto 协议指针
 * @param callback 回调函数
 * @param arg 回调参数
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_uart_protocol_set_callback(kt_uart_protocol_t* proto, 
                                          kt_uart_rx_callback_t callback, void* arg);

/**
 * @brief 处理接收到的数据（需要在主循环中调用）
 * @param proto 协议指针
 * @param data 数据指针
 * @param len 数据长度
 */
void kt_uart_protocol_receive(kt_uart_protocol_t* proto, const kt_uint8_t* data, kt_uint16_t len);

/**
 * @brief 发送数据帧
 * @param proto 协议指针
 * @param cmd 命令字
 * @param data 数据指针
 * @param len 数据长度
 * @return 实际发送的字节数
 */
kt_int32_t kt_uart_protocol_send(kt_uart_protocol_t* proto, kt_uint8_t cmd, 
                                 const kt_uint8_t* data, kt_uint16_t len);

/**
 * @brief 重置协议状态
 * @param proto 协议指针
 */
void kt_uart_protocol_reset(kt_uart_protocol_t* proto);

/**
 * @brief 获取错误计数
 * @param proto 协议指针
 * @return 错误计数
 */
kt_uint32_t kt_uart_protocol_get_error_count(kt_uart_protocol_t* proto);

#endif /* KT_DRIVER_UART_PROTOCOL_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_UART_PROTOCOL_H */