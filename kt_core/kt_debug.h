/**
 * @file kt_debug.h
 * @author kento
 * @brief 调试工具
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件提供调试打印和断言功能
 */

#ifndef KT_DEBUG_H
#define KT_DEBUG_H

#include "kt_config.h"
#include "kt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                         调试开关
 *============================================================================*/

#ifdef KT_DEBUG_ENABLE

/*============================================================================
 *                         调试打印级别
 *============================================================================*/

/**
 * @brief 调试打印级别
 */
typedef enum
{
    KT_LOG_LEVEL_NONE = 0,    /**< 不输出 */
    KT_LOG_LEVEL_ERROR,       /**< 错误 */
    KT_LOG_LEVEL_WARN,        /**< 警告 */
    KT_LOG_LEVEL_INFO,        /**< 信息 */
    KT_LOG_LEVEL_DEBUG        /**< 调试 */
} kt_log_level_t;

/**
 * @brief 当前调试级别
 * @note 可通过修改此值控制输出级别
 */
extern kt_log_level_t kt_debug_level;

/*============================================================================
 *                         调试打印宏
 *============================================================================*/

/**
 * @brief 打印错误信息（红色）
 */
#define KT_LOG_ERROR(fmt, ...) \
    do { \
        if (kt_debug_level >= KT_LOG_LEVEL_ERROR) { \
            KT_DEBUG_PRINTF("[E][%s:%d] " fmt "\n", \
                           __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief 打印警告信息（黄色）
 */
#define KT_LOG_WARN(fmt, ...) \
    do { \
        if (kt_debug_level >= KT_LOG_LEVEL_WARN) { \
            KT_DEBUG_PRINTF("[W][%s:%d] " fmt "\n", \
                           __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief 打印信息（绿色）
 */
#define KT_LOG_INFO(fmt, ...) \
    do { \
        if (kt_debug_level >= KT_LOG_LEVEL_INFO) { \
            KT_DEBUG_PRINTF("[I][%s:%d] " fmt "\n", \
                           __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief 打印调试信息（蓝色）
 */
#define KT_LOG_DEBUG(fmt, ...) \
    do { \
        if (kt_debug_level >= KT_LOG_LEVEL_DEBUG) { \
            KT_DEBUG_PRINTF("[D][%s:%d] " fmt "\n", \
                           __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

/**
 * @brief 打印十六进制数据
 * @param data 数据指针
 * @param len 数据长度
 */
void kt_debug_hex_dump(const void* data, kt_uint32_t len);

/*============================================================================
 *                         断言宏
 *============================================================================*/

/**
 * @brief 断言失败处理函数
 */
void kt_assert_failed(const char* file, int line, const char* func, const char* expr);

/**
 * @brief 断言宏
 * @note 如果条件不满足，将调用kt_assert_failed并进入死循环
 */
#define KT_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            kt_assert_failed(__FILE__, __LINE__, __FUNCTION__, #expr); \
            while(1); \
        } \
    } while(0)

/**
 * @brief 参数检查断言
 * @note 用于函数参数检查，失败时返回错误码
 */
#define KT_ASSERT_PARAM(expr) \
    do { \
        if (!(expr)) { \
            KT_LOG_ERROR("Parameter check failed: %s", #expr); \
            return KT_ERR_INVALID_PARAM; \
        } \
    } while(0)

/*============================================================================
 *                         性能分析宏（可选）
 *============================================================================*/

#ifdef KT_DEBUG_PERF_ENABLE

/**
 * @brief 开始计时
 */
#define KT_PERF_START(name) \
    kt_uint32_t _perf_start_##name = kt_port_get_tick()

/**
 * @brief 结束计时并打印
 */
#define KT_PERF_END(name) \
    do { \
        kt_uint32_t _perf_end_##name = kt_port_get_tick(); \
        KT_LOG_DEBUG("PERF[%s]: %lu ms", #name, \
                    (_perf_end_##name - _perf_start_##name)); \
    } while(0)

#endif /* KT_DEBUG_PERF_ENABLE */

#else /* !KT_DEBUG_ENABLE */

/*============================================================================
 *                         非调试模式（空宏）
 *============================================================================*/

#define KT_LOG_ERROR(fmt, ...)      ((void)0)
#define KT_LOG_WARN(fmt, ...)       ((void)0)
#define KT_LOG_INFO(fmt, ...)       ((void)0)
#define KT_LOG_DEBUG(fmt, ...)      ((void)0)

#define kt_debug_hex_dump(data, len)    ((void)0)

#define KT_ASSERT(expr)             ((void)0)
#define KT_ASSERT_PARAM(expr) \
    do { \
        if (!(expr)) { \
            return KT_ERR_INVALID_PARAM; \
        } \
    } while(0)

#define kt_assert_failed(file, line, func, expr)    ((void)0)

#ifdef KT_DEBUG_PERF_ENABLE
#define KT_PERF_START(name)     ((void)0)
#define KT_PERF_END(name)       ((void)0)
#endif

#endif /* KT_DEBUG_ENABLE */

/*============================================================================
 *                         调试辅助函数
 *============================================================================*/

/**
 * @brief 获取调试级别字符串
 * @param level 调试级别
 * @return 级别字符串
 */
const char* kt_debug_level_to_string(kt_log_level_t level);

/**
 * @brief 设置调试级别
 * @param level 调试级别
 */
void kt_debug_set_level(kt_log_level_t level);

/**
 * @brief 获取当前调试级别
 * @return 当前调试级别
 */
kt_log_level_t kt_debug_get_level(void);

#ifdef __cplusplus
}
#endif

#endif /* KT_DEBUG_H */