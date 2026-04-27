/**
 * @file kt_ringbuf.c
 * @author kento
 * @brief 环形缓冲区实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_ringbuf.h"

#if KT_COMPONENT_RINGBUF_ENABLE

/**
 * @brief 初始化环形缓冲区
 */
kt_status_t kt_ringbuf_init(kt_ringbuf_t* rb, kt_uint8_t* buffer, kt_uint32_t size)
{
    if (rb == NULL || buffer == NULL || size == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    rb->buffer = buffer;
    rb->size = size;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->full = KT_FALSE;
    
    return KT_OK;
}

/**
 * @brief 重置环形缓冲区
 */
void kt_ringbuf_reset(kt_ringbuf_t* rb)
{
    if (rb == NULL)
    {
        return;
    }
    
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->full = KT_FALSE;
}

/**
 * @brief 写入数据
 */
kt_uint32_t kt_ringbuf_write(kt_ringbuf_t* rb, const kt_uint8_t* data, kt_uint32_t length)
{
    kt_uint32_t i;
    kt_uint32_t written = 0;
    
    if (rb == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        if (rb->full)
        {
            /* 缓冲区满，停止写入 */
            break;
        }
        
        rb->buffer[rb->write_pos] = data[i];
        rb->write_pos++;
        
        if (rb->write_pos >= rb->size)
        {
            rb->write_pos = 0;
        }
        
        written++;
        
        /* 检查是否满了 */
        if (rb->write_pos == rb->read_pos)
        {
            rb->full = KT_TRUE;
        }
    }
    
    return written;
}

/**
 * @brief 读取数据
 */
kt_uint32_t kt_ringbuf_read(kt_ringbuf_t* rb, kt_uint8_t* data, kt_uint32_t length)
{
    kt_uint32_t i;
    kt_uint32_t read = 0;
    
    if (rb == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        if (rb->read_pos == rb->write_pos && !rb->full)
        {
            /* 缓冲区空，停止读取 */
            break;
        }
        
        data[i] = rb->buffer[rb->read_pos];
        rb->read_pos++;
        
        if (rb->read_pos >= rb->size)
        {
            rb->read_pos = 0;
        }
        
        read++;
        
        /* 读取后不再满 */
        rb->full = KT_FALSE;
    }
    
    return read;
}

/**
 * @brief 获取缓冲区中的数据量
 */
kt_uint32_t kt_ringbuf_data_len(kt_ringbuf_t* rb)
{
    if (rb == NULL)
    {
        return 0;
    }
    
    if (rb->full)
    {
        return rb->size;
    }
    
    if (rb->write_pos >= rb->read_pos)
    {
        return rb->write_pos - rb->read_pos;
    }
    else
    {
        return rb->size - rb->read_pos + rb->write_pos;
    }
}

/**
 * @brief 获取缓冲区剩余可用空间
 */
kt_uint32_t kt_ringbuf_free_space(kt_ringbuf_t* rb)
{
    if (rb == NULL)
    {
        return 0;
    }
    
    return rb->size - kt_ringbuf_data_len(rb);
}

/**
 * @brief 检查缓冲区是否为空
 */
kt_bool_t kt_ringbuf_is_empty(kt_ringbuf_t* rb)
{
    if (rb == NULL)
    {
        return KT_TRUE;
    }
    
    return (rb->read_pos == rb->write_pos && !rb->full) ? KT_TRUE : KT_FALSE;
}

/**
 * @brief 检查缓冲区是否已满
 */
kt_bool_t kt_ringbuf_is_full(kt_ringbuf_t* rb)
{
    if (rb == NULL)
    {
        return KT_FALSE;
    }
    
    return rb->full;
}

/**
 * @brief Peek数据（读取但不移动读指针）
 */
kt_uint32_t kt_ringbuf_peek(kt_ringbuf_t* rb, kt_uint8_t* data, kt_uint32_t length)
{
    kt_uint32_t i;
    kt_uint32_t peeked = 0;
    kt_uint32_t pos;
    
    if (rb == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    pos = rb->read_pos;
    
    for (i = 0; i < length; i++)
    {
        if (pos == rb->write_pos && !rb->full)
        {
            break;
        }
        
        data[i] = rb->buffer[pos];
        pos++;
        
        if (pos >= rb->size)
        {
            pos = 0;
        }
        
        peeked++;
    }
    
    return peeked;
}

#endif /* KT_COMPONENT_RINGBUF_ENABLE */