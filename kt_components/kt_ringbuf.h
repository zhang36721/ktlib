/**
 * @file kt_ringbuf.h
 * @author kento
 * @brief 环形缓冲区
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_RINGBUF_H
#define KT_RINGBUF_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_COMPONENT_RINGBUF_ENABLE

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief 环形缓冲区结构体
 */
typedef struct
{
    kt_uint8_t* buffer;     /**< 数据缓冲区 */
    kt_uint32_t size;       /**< 缓冲区大小 */
    kt_uint32_t read_pos;   /**< 读位置 */
    kt_uint32_t write_pos;  /**< 写位置 */
    kt_bool_t   full;       /**< 是否满 */
} kt_ringbuf_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化环形缓冲区
 * @param rb 环形缓冲区指针
 * @param buffer 数据缓冲区
 * @param size 缓冲区大小
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_ringbuf_init(kt_ringbuf_t* rb, kt_uint8_t* buffer, kt_uint32_t size);

/**
 * @brief 重置环形缓冲区
 * @param rb 环形缓冲区指针
 */
void kt_ringbuf_reset(kt_ringbuf_t* rb);

/**
 * @brief 写入数据
 * @param rb 环形缓冲区指针
 * @param data 数据指针
 * @param length 数据长度
 * @return 实际写入的字节数
 */
kt_uint32_t kt_ringbuf_write(kt_ringbuf_t* rb, const kt_uint8_t* data, kt_uint32_t length);

/**
 * @brief 读取数据
 * @param rb 环形缓冲区指针
 * @param data 数据指针
 * @param length 数据长度
 * @return 实际读取的字节数
 */
kt_uint32_t kt_ringbuf_read(kt_ringbuf_t* rb, kt_uint8_t* data, kt_uint32_t length);

/**
 * @brief 获取缓冲区中的数据量
 * @param rb 环形缓冲区指针
 * @return 数据量（字节数）
 */
kt_uint32_t kt_ringbuf_data_len(kt_ringbuf_t* rb);

/**
 * @brief 获取缓冲区剩余可用空间
 * @param rb 环形缓冲区指针
 * @return 剩余空间（字节数）
 */
kt_uint32_t kt_ringbuf_free_space(kt_ringbuf_t* rb);

/**
 * @brief 检查缓冲区是否为空
 * @param rb 环形缓冲区指针
 * @return KT_TRUE 空，KT_FALSE 非空
 */
kt_bool_t kt_ringbuf_is_empty(kt_ringbuf_t* rb);

/**
 * @brief 检查缓冲区是否已满
 * @param rb 环形缓冲区指针
 * @return KT_TRUE 满，KT_FALSE 未满
 */
kt_bool_t kt_ringbuf_is_full(kt_ringbuf_t* rb);

/**
 * @brief  Peek数据（读取但不移动读指针）
 * @param rb 环形缓冲区指针
 * @param data 数据指针
 * @param length 数据长度
 * @return 实际Peek的字节数
 */
kt_uint32_t kt_ringbuf_peek(kt_ringbuf_t* rb, kt_uint8_t* data, kt_uint32_t length);

#endif /* KT_COMPONENT_RINGBUF_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_RINGBUF_H */