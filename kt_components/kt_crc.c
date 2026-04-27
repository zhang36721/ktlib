/**
 * @file kt_crc.c
 * @author kento
 * @brief CRC校验实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_crc.h"

#if KT_COMPONENT_CRC_ENABLE

/**
 * @brief 计算CRC8
 */
kt_uint8_t kt_crc8(const kt_uint8_t* data, kt_uint32_t length, 
                   kt_uint8_t poly, kt_uint8_t init)
{
    kt_uint8_t crc = init;
    kt_uint32_t i, j;
    
    if (data == NULL || length == 0)
    {
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

/**
 * @brief 计算CRC8（SMBus）
 */
kt_uint8_t kt_crc8_smbus(const kt_uint8_t* data, kt_uint32_t length)
{
    return kt_crc8(data, length, KT_CRC8_POLY_SMBUS, KT_CRC8_INIT);
}

/**
 * @brief 计算CRC16
 */
kt_uint16_t kt_crc16(const kt_uint8_t* data, kt_uint32_t length,
                     kt_uint16_t poly, kt_uint16_t init)
{
    kt_uint16_t crc = init;
    kt_uint32_t i, j;
    
    if (data == NULL || length == 0)
    {
        return 0;
    }
    
    for (i = 0; i < length; i++)
    {
        crc ^= (kt_uint16_t)data[i] << 8;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

/**
 * @brief 计算CRC16（IBM）
 */
kt_uint16_t kt_crc16_ibm(const kt_uint8_t* data, kt_uint32_t length)
{
    return kt_crc16(data, length, KT_CRC16_POLY_IBM, KT_CRC16_INIT);
}

/**
 * @brief 计算CRC16（CCITT）
 */
kt_uint16_t kt_crc16_ccitt(const kt_uint8_t* data, kt_uint32_t length)
{
    return kt_crc16(data, length, KT_CRC16_POLY_CCITT, 0xFFFF);
}

/**
 * @brief 计算CRC32
 */
kt_uint32_t kt_crc32(const kt_uint8_t* data, kt_uint32_t length, kt_uint32_t init)
{
    kt_uint32_t crc = init;
    kt_uint32_t i, j;
    
    if (data == NULL || length == 0)
    {
        return 0;
    }
    
    crc = ~crc;
    
    for (i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ KT_CRC32_POLY;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    
    return ~crc;
}

/**
 * @brief 验证CRC8
 */
kt_bool_t kt_crc8_verify(const kt_uint8_t* data, kt_uint32_t length,
                         kt_uint8_t poly, kt_uint8_t init)
{
    kt_uint8_t crc;
    
    if (data == NULL || length == 0)
    {
        return KT_FALSE;
    }
    
    crc = kt_crc8(data, length, poly, init);
    
    return (crc == 0) ? KT_TRUE : KT_FALSE;
}

/**
 * @brief 验证CRC16
 */
kt_bool_t kt_crc16_verify(const kt_uint8_t* data, kt_uint32_t length,
                          kt_uint16_t poly, kt_uint16_t init)
{
    kt_uint16_t crc;
    
    if (data == NULL || length == 0)
    {
        return KT_FALSE;
    }
    
    crc = kt_crc16(data, length, poly, init);
    
    return (crc == 0) ? KT_TRUE : KT_FALSE;
}

/**
 * @brief 验证CRC32
 */
kt_bool_t kt_crc32_verify(const kt_uint8_t* data, kt_uint32_t length, kt_uint32_t init)
{
    kt_uint32_t crc;
    
    if (data == NULL || length == 0)
    {
        return KT_FALSE;
    }
    
    crc = kt_crc32(data, length, init);
    
    return (crc == 0) ? KT_TRUE : KT_FALSE;
}

#endif /* KT_COMPONENT_CRC_ENABLE */