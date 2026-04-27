/**
 * @file kt_types.h
 * @author kento
 * @brief 基础类型定义
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件定义了ktlib库中使用的统一数据类型
 */

#ifndef KT_TYPES_H
#define KT_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                           基本整数类型
 *============================================================================*/

/* 有符号整数类型 */
typedef int8_t      kt_int8_t;
typedef int16_t     kt_int16_t;
typedef int32_t     kt_int32_t;
typedef int64_t     kt_int64_t;

/* 无符号整数类型 */
typedef uint8_t     kt_uint8_t;
typedef uint16_t    kt_uint16_t;
typedef uint32_t    kt_uint32_t;
typedef uint64_t    kt_uint64_t;

/* 常用别名 */
typedef kt_uint8_t  kt_u8;
typedef kt_uint16_t kt_u16;
typedef kt_uint32_t kt_u32;
typedef kt_uint64_t kt_u64;

typedef kt_int8_t   kt_s8;
typedef kt_int16_t  kt_s16;
typedef kt_int32_t  kt_s32;
typedef kt_int64_t  kt_s64;

/*============================================================================
 *                           字符与字符串类型
 *============================================================================*/

/* 字符类型 */
typedef char        kt_char_t;

/* 字符串指针类型 */
typedef const char* kt_string_t;
typedef char*       kt_char_ptr_t;

/*============================================================================
 *                           浮点数类型
 *============================================================================*/

#include <float.h>

typedef float       kt_float_t;
typedef double      kt_double_t;

/*============================================================================
 *                           布尔类型
 *============================================================================*/

#ifndef __cplusplus
#ifndef bool
typedef enum
{
    KT_FALSE = 0,   /**< 假 */
    KT_TRUE  = 1    /**< 真 */
} kt_bool_t;
#else
typedef bool        kt_bool_t;
#define KT_FALSE    false
#define KT_TRUE     true
#endif /* __cplusplus */
#endif /* bool */

/*============================================================================
 *                           状态与错误类型
 *============================================================================*/

/**
 * @brief 通用状态类型
 */
typedef int32_t     kt_status_t;

/*============================================================================
 *                           时间与日期类型
 *============================================================================*/

/**
 * @brief 时间戳类型（毫秒）
 */
typedef uint32_t    kt_tick_t;

/**
 * @brief 时间结构体
 */
typedef struct
{
    kt_uint16_t year;   /**< 年 */
    kt_uint8_t  month;  /**< 月 (1-12) */
    kt_uint8_t  day;    /**< 日 (1-31) */
    kt_uint8_t  hour;   /**< 时 (0-23) */
    kt_uint8_t  minute; /**< 分 (0-59) */
    kt_uint8_t  second; /**< 秒 (0-59) */
} kt_time_t;

/*============================================================================
 *                           通用结构体
 *============================================================================*/

/**
 * @brief 2D点坐标
 */
typedef struct
{
    kt_int32_t x;
    kt_int32_t y;
} kt_point_t;

/**
 * @brief 矩形区域
 */
typedef struct
{
    kt_int32_t x;
    kt_int32_t y;
    kt_uint32_t width;
    kt_uint32_t height;
} kt_rect_t;

/**
 * @brief 尺寸
 */
typedef struct
{
    kt_uint32_t width;
    kt_uint32_t height;
} kt_size_t;

/*============================================================================
 *                           函数指针类型
 *============================================================================*/

/**
 * @brief 无参数无返回值函数指针
 */
typedef void (*kt_void_func_t)(void);

/**
 * @brief 回调函数类型（带一个参数）
 */
typedef void (*kt_callback1_t)(void* arg);

/**
 * @brief 回调函数类型（带两个参数）
 */
typedef void (*kt_callback2_t)(void* arg1, void* arg2);

/*============================================================================
 *                           宏定义工具
 *============================================================================*/

/** 获取结构体成员偏移量 */
#ifndef offsetof
#define offsetof(type, member)  ((size_t)&(((type*)0)->member))
#endif

/** 通过成员指针获取结构体起始地址 */
#ifndef container_of
#define container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))
#endif

/** 获取数组元素个数 */
#ifndef KT_ARRAY_SIZE
#define KT_ARRAY_SIZE(arr)  (sizeof(arr) / sizeof((arr)[0]))
#endif

/** 取最小值 */
#ifndef KT_MIN
#define KT_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

/** 取最大值 */
#ifndef KT_MAX
#define KT_MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

/** 限制值在范围内 */
#ifndef KT_CONSTRAIN
#define KT_CONSTRAIN(amt, low, high) \
    ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif

/** 交换两个值 */
#ifndef KT_SWAP
#define KT_SWAP(a, b)  do { typeof(a) _tmp = (a); (a) = (b); (b) = _tmp; } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* KT_TYPES_H */