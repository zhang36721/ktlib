/**
 * @file kt_crc.h
 * @author kento
 * @brief CRC校验
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_CRC_H
#define KT_CRC_H

#include "kt_types.h"
#include "kt_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_COMPONENT_CRC_ENABLE

/*============================================================================
 *                         CRC多项式定义
 *============================================================================*/

/** CRC8 多项式 */
#define KT_CRC8_POLY        0x07
#define KT_CRC8_INIT        0x00

/** CRC8 常用多项式 */
#define KT_CRC8_POLY_SMBUS  0x07    /* SMBus CRC8 */
#define KT_CRC8_POLY_Dallas 0x31    /* Dallas/Maxim CRC8 */

/** CRC16 多项式 */
#define KT_CRC16_POLY       0x8005
#define KT_CRC16_INIT       0x0000

/** CRC16 常用多项式 */
#define KT_CRC16_POLY_IBM   0x8005  /* CRC16-IBM */
#define KT_CRC16_POLY_CCITT 0x1021  /* CRC16-CCITT */
#define KT_CRC16_POLY_XMODEM 0x1021 /* XMODEM CRC16 */

/** CRC32 多项式 */
#define KT_CRC32_POLY       0x04C11DB7UL
#define KT_CRC32_INIT       0xFFFFFFFFUL

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 计算CRC8
 * @param data 数据指针
 * @param length 数据长度
 * @param poly 多项式
 * @param init 初始值
 * @return CRC8结果
 */
kt_uint8_t kt_crc8(const kt_uint8_t* data, kt_uint32_t length, 
                   kt_uint8_t poly, kt_uint8_t init);

/**
 * @brief 计算CRC8（SMBus）
 * @param data 数据指针
 * @param length 数据长度
 * @return CRC8结果
 */
kt_uint8_t kt_crc8_smbus(const kt_uint8_t* data, kt_uint32_t length);

/**
 * @brief 计算CRC16
 * @param data 数据指针
 * @param length 数据长度
 * @param poly 多项式
 * @param init 初始值
 * @return CRC16结果
 */
kt_uint16_t kt_crc16(const kt_uint8_t* data, kt_uint32_t length,
                     kt_uint16_t poly, kt_uint16_t init);

/**
 * @brief 计算CRC16（IBM）
 * @param data 数据指针
 * @param length 数据长度
 * @return CRC16结果
 */
kt_uint16_t kt_crc16_ibm(const kt_uint8_t* data, kt_uint32_t length);

/**
 * @brief 计算CRC16（CCITT）
 * @param data 数据指针
 * @param length 数据长度
 * @return CRC16结果
 */
kt_uint16_t kt_crc16_ccitt(const kt_uint8_t* data, kt_uint32_t length);

/**
 * @brief 计算CRC32
 * @param data 数据指针
 * @param length 数据长度
 * @param init 初始值
 * @return CRC32结果
 */
kt_uint32_t kt_crc32(const kt_uint8_t* data, kt_uint32_t length, kt_uint32_t init);

/**
 * @brief 验证CRC8
 * @param data 数据指针
 * @param length 数据长度（包含CRC字节）
 * @param poly 多项式
 * @param init 初始值
 * @return KT_TRUE 校验通过，KT_FALSE 校验失败
 */
kt_bool_t kt_crc8_verify(const kt_uint8_t* data, kt_uint32_t length,
                         kt_uint8_t poly, kt_uint8_t init);

/**
 * @brief 验证CRC16
 * @param data 数据指针
 * @param length 数据长度（包含CRC字节）
 * @param poly 多项式
 * @param init 初始值
 * @return KT_TRUE 校验通过，KT_FALSE 校验失败
 */
kt_bool_t kt_crc16_verify(const kt_uint8_t* data, kt_uint32_t length,
                          kt_uint16_t poly, kt_uint16_t init);

/**
 * @brief 验证CRC32
 * @param data 数据指针
 * @param length 数据长度（包含CRC字节）
 * @param init 初始值
 * @return KT_TRUE 校验通过，KT_FALSE 校验失败
 */
kt_bool_t kt_crc32_verify(const kt_uint8_t* data, kt_uint32_t length, kt_uint32_t init);

#endif /* KT_COMPONENT_CRC_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_CRC_H */