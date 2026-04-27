/**
 * @file kt_utils.h
 * @author kento
 * @brief 工具函数
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件提供常用的工具函数声明
 */

#ifndef KT_UTILS_H
#define KT_UTILS_H

#include "kt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                         位操作宏
 *============================================================================*/

/** 设置指定位 */
#define KT_BIT_SET(reg, bit)        ((reg) |= (1U << (bit)))

/** 清除指定位 */
#define KT_BIT_CLEAR(reg, bit)      ((reg) &= ~(1U << (bit)))

/** 翻转指定位 */
#define KT_BIT_TOGGLE(reg, bit)     ((reg) ^= (1U << (bit)))

/** 读取指定位 */
#define KT_BIT_READ(reg, bit)       (((reg) >> (bit)) & 1U)

/** 写入指定位 */
#define KT_BIT_WRITE(reg, bit, val) \
    ((reg) = ((reg) & ~(1U << (bit))) | ((val) << (bit)))

/** 检查位是否设置 */
#define KT_BIT_IS_SET(reg, bit)     (((reg) & (1U << (bit))) != 0)

/** 位域设置 */
#define KT_BITFIELD_SET(reg, pos, mask, val) \
    ((reg) = ((reg) & ~((mask) << (pos))) | (((val) & (mask)) << (pos)))

/** 位域读取 */
#define KT_BITFIELD_GET(reg, pos, mask) \
    (((reg) >> (pos)) & (mask))

/*============================================================================
 *                         字节操作宏
 *============================================================================*/

/** 高字节 */
#define KT_HI_BYTE(val)             (((val) >> 8) & 0xFF)

/** 低字节 */
#define KT_LO_BYTE(val)             ((val) & 0xFF)

/** 高16位 */
#define KT_HI_WORD(val)             (((val) >> 16) & 0xFFFF)

/** 低16位 */
#define KT_LO_WORD(val)             ((val) & 0xFFFF)

/** 合并两个字节 */
#define KT_MAKE_WORD(hi, lo)        ((((kt_uint16_t)(hi)) << 8) | (lo))

/** 合并两个16位字 */
#define KT_MAKE_DWORD(hi, lo)       ((((kt_uint32_t)(hi)) << 16) | (lo))

/*============================================================================
 *                         大小端转换
 *============================================================================*/

/**
 * @brief 16位值字节交换（大端转小端或小端转大端）
 */
static inline kt_uint16_t kt_swap16(kt_uint16_t val)
{
    return (val << 8) | (val >> 8);
}

/**
 * @brief 32位值字节交换
 */
static inline kt_uint32_t kt_swap32(kt_uint32_t val)
{
    return ((val << 24) | ((val >> 8) << 16) | ((val >> 16) << 8) | (val >> 24));
}

/**
 * @brief 主机字节序转网络字节序（16位）
 */
static inline kt_uint16_t kt_htons(kt_uint16_t val)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return kt_swap16(val);
#else
    return val;
#endif
}

/**
 * @brief 网络字节序转主机字节序（16位）
 */
#define kt_ntohs(val)     kt_htons(val)

/**
 * @brief 主机字节序转网络字节序（32位）
 */
static inline kt_uint32_t kt_htonl(kt_uint32_t val)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return kt_swap32(val);
#else
    return val;
#endif
}

/**
 * @brief 网络字节序转主机字节序（32位）
 */
#define kt_ntohl(val)     kt_htonl(val)

/*============================================================================
 *                         数学运算
 *============================================================================*/

/**
 * @brief 绝对值
 */
static inline kt_int32_t kt_abs(kt_int32_t val)
{
    return (val < 0) ? -val : val;
}

/**
 * @brief 限制值在范围内
 */
static inline kt_int32_t kt_constrain(kt_int32_t val, kt_int32_t min, kt_int32_t max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

/**
 * @brief 映射值从一个范围到另一个范围
 */
static inline kt_int32_t kt_map(kt_int32_t val, 
                                 kt_int32_t in_min, kt_int32_t in_max,
                                 kt_int32_t out_min, kt_int32_t out_max)
{
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief 线性插值
 */
static inline kt_float_t kt_lerp(kt_float_t a, kt_float_t b, kt_float_t t)
{
    return a + (b - a) * t;
}

/*============================================================================
 *                         字符串操作
 *============================================================================*/

/**
 * @brief 字符串长度（不包含终止符）
 */
kt_uint32_t kt_strlen(const char* str);

/**
 * @brief 字符串复制
 */
char* kt_strcpy(char* dest, const char* src);

/**
 * @brief 字符串连接
 */
char* kt_strcat(char* dest, const char* src);

/**
 * @brief 字符串比较
 */
kt_int32_t kt_strcmp(const char* s1, const char* s2);

/**
 * @brief 字符串转整数
 */
kt_int32_t kt_atoi(const char* str);

/**
 * @brief 整数转字符串
 */
char* kt_itoa(kt_int32_t val, char* str, kt_int32_t radix);

/*============================================================================
 *                         内存操作
 *============================================================================*/

/**
 * @brief 内存设置
 */
void* kt_memset(void* dest, kt_int32_t val, kt_uint32_t count);

/**
 * @brief 内存复制
 */
void* kt_memcpy(void* dest, const void* src, kt_uint32_t count);

/**
 * @brief 内存比较
 */
kt_int32_t kt_memcmp(const void* s1, const void* s2, kt_uint32_t count);

/**
 * @brief 内存移动
 */
void* kt_memmove(void* dest, const void* src, kt_uint32_t count);

/*============================================================================
 *                         数组操作
 *============================================================================*/

/**
 * @brief 数组反转
 * @param arr 数组指针
 * @param len 数组长度
 */
void kt_array_reverse(void* arr, kt_uint32_t len, kt_uint32_t elem_size);

/**
 * @brief 查找数组中的元素
 * @param arr 数组指针
 * @param len 数组长度
 * @param elem_size 元素大小
 * @param target 目标元素指针
 * @param cmp 比较函数
 * @return 找到返回索引，否则返回-1
 */
kt_int32_t kt_array_find(const void* arr, kt_uint32_t len, kt_uint32_t elem_size,
                         const void* target, 
                         kt_int32_t (*cmp)(const void*, const void*));

/*============================================================================
 *                         CRC计算（快速版本）
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
 * @brief 计算CRC32
 * @param data 数据指针
 * @param length 数据长度
 * @param init 初始值
 * @return CRC32结果
 */
kt_uint32_t kt_crc32(const kt_uint8_t* data, kt_uint32_t length, kt_uint32_t init);

/*============================================================================
 *                         工具函数
 *============================================================================*/

/**
 * @brief 获取版本号字符串
 * @return 版本号字符串
 */
const char* kt_get_version(void);

/**
 * @brief 获取库的描述信息
 * @return 描述信息
 */
const char* kt_get_description(void);

#ifdef __cplusplus
}
#endif

#endif /* KT_UTILS_H */