/**
 * @file kt_config.h
 * @author kento
 * @brief 配置宏定义
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件包含ktlib库的可配置宏定义
 */

#ifndef KT_CONFIG_H
#define KT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                         版本信息配置
 *============================================================================*/

/** 主版本号 */
#define KT_VERSION_MAJOR    0

/** 次版本号 */
#define KT_VERSION_MINOR    1

/** 修订版本号 */
#define KT_VERSION_PATCH    0

/** 版本字符串 */
#define KT_VERSION_STRING   "0.1.0"

/*============================================================================
 *                         调试功能配置
 *============================================================================*/

/** 
 * @brief 启用调试模式
 * @note 取消注释以启用调试功能（如调试打印、断言等）
 */
/* #define KT_DEBUG_ENABLE */

/**
 * @brief 调试输出函数
 * @note 默认使用printf，可根据平台修改
 */
#ifndef KT_DEBUG_PRINTF
#include <stdio.h>
#define KT_DEBUG_PRINTF     printf
#endif

/*============================================================================
 *                         组件功能开关
 *============================================================================*/

/**
 * @brief 启用环形缓冲区组件
 */
#define KT_COMPONENT_RINGBUF_ENABLE     1

/**
 * @brief 启用软件定时器组件
 */
#define KT_COMPONENT_SOFT_TIMER_ENABLE  1

/**
 * @brief 启用CRC校验组件
 */
#define KT_COMPONENT_CRC_ENABLE         1

/*============================================================================
 *                         驱动功能开关
 *============================================================================*/

/**
 * @brief 启用LED驱动
 */
#define KT_DRIVER_LED_ENABLE            1

/**
 * @brief 启用按键驱动
 */
#define KT_DRIVER_BUTTON_ENABLE         1

/**
 * @brief 启用蜂鸣器驱动
 */
#define KT_DRIVER_BUZZER_ENABLE         1

/**
 * @brief 启用UART协议
 */
#define KT_DRIVER_UART_PROTOCOL_ENABLE  1

/*============================================================================
 *                         组件参数配置
 *============================================================================*/

/*---------------------- 环形缓冲区配置 ----------------------*/

/** 环形缓冲区默认大小 */
#define KT_RINGBUF_DEFAULT_SIZE         256

/*---------------------- 软件定时器配置 ----------------------*/

/** 最大软件定时器数量 */
#define KT_SOFT_TIMER_MAX_NUM           8

/** 软件定时器时间间隔（毫秒） */
#define KT_SOFT_TIMER_INTERVAL_MS       10

/*---------------------- CRC配置 ----------------------*/

/** CRC数据表大小（用于CRC8/CRC16查表法） */
#define KT_CRC_TABLE_SIZE               256

/*---------------------- 按键驱动配置 ----------------------*/

/** 按键扫描间隔（毫秒） */
#define KT_BUTTON_SCAN_INTERVAL_MS      10

/** 长按判定时间（毫秒） */
#define KT_BUTTON_LONG_PRESS_MS         1000

/** 双击判定时间间隔（毫秒） */
#define KT_BUTTON_DOUBLE_CLICK_MS       300

/*---------------------- LED驱动配置 ----------------------*/

/** 最大LED数量 */
#define KT_LED_MAX_NUM                  8

/** LED呼吸效果步数 */
#define KT_LED_BREATH_STEPS             100

/*---------------------- 蜂鸣器驱动配置 ----------------------*/

/** 最大蜂鸣器数量 */
#define KT_BUZZER_MAX_NUM               2

/*============================================================================
 *                         平台选择配置
 *============================================================================*/

/**
 * @brief 平台选择
 * @note 取消注释以选择对应平台，默认使用Mock平台
 * 
 * 可选平台：
 * - USE_KT_PLATFORM_STM32_HAL   STM32 HAL库平台
 * - USE_KT_PLATFORM_ESP32       ESP32平台
 * - USE_KT_PLATFORM_8051        8051平台
 * - USE_KT_PLATFORM_MOCK        Mock模拟平台（默认）
 */

/* #define USE_KT_PLATFORM_STM32_HAL */
/* #define USE_KT_PLATFORM_ESP32 */
/* #define USE_KT_PLATFORM_8051 */

#if !defined(USE_KT_PLATFORM_STM32_HAL) && \
    !defined(USE_KT_PLATFORM_ESP32) && \
    !defined(USE_KT_PLATFORM_8051)
    #define USE_KT_PLATFORM_MOCK
#endif

/*============================================================================
 *                         内存管理配置
 *============================================================================*/

/**
 * @brief 动态内存分配函数
 * @note 默认使用标准库的malloc/free，可替换为自定义内存管理器
 */
#ifndef KT_MALLOC
#include <stdlib.h>
#define KT_MALLOC(size)         malloc(size)
#endif

#ifndef KT_FREE
#define KT_FREE(ptr)            free(ptr)
#endif

#ifndef KT_CALLOC
#define KT_CALLOC(n, size)      calloc(n, size)
#endif

#ifndef KT_REALLOC
#define KT_REALLOC(ptr, size)   realloc(ptr, size)
#endif

/*============================================================================
 *                         中断保护配置
 *============================================================================*/

/**
 * @brief 进入临界区（关中断）
 * @note 根据平台实现，确保原子操作
 */
#ifndef KT_ENTER_CRITICAL
#define KT_ENTER_CRITICAL()     /* 根据平台实现 */
#endif

/**
 * @brief 退出临界区（开中断）
 */
#ifndef KT_EXIT_CRITICAL()
#define KT_EXIT_CRITICAL()      /* 根据平台实现 */
#endif

/*============================================================================
 *                         对齐配置
 *============================================================================*/

/**
 * @brief 数据对齐字节数
 * @note 某些平台可能需要4字节或8字节对齐
 */
#define KT_ALIGNMENT            4

/**
 * @brief 对齐宏
 */
#define KT_ALIGN_UP(x, align)   (((x) + ((align) - 1)) & ~((align) - 1))
#define KT_ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

/*============================================================================
 *                         其他配置
 *============================================================================*/

/**
 * @brief 是否使用64位时间戳
 * @note 某些应用可能需要更长的时间范围
 */
#define KT_USE_64BIT_TICK       0

/**
 * @brief 默认UART波特率
 */
#define KT_UART_DEFAULT_BAUDRATE    115200

/**
 * @brief 默认SPI时钟频率（Hz）
 */
#define KT_SPI_DEFAULT_FREQ         1000000

#ifdef __cplusplus
}
#endif

#endif /* KT_CONFIG_H */