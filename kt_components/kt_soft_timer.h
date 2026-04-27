/**
 * @file kt_soft_timer.h
 * @author kento
 * @brief 软件定时器
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_SOFT_TIMER_H
#define KT_SOFT_TIMER_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_COMPONENT_SOFT_TIMER_ENABLE

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief 定时器状态
 */
typedef enum
{
    KT_TIMER_STATE_STOPPED = 0,   /**< 停止 */
    KT_TIMER_STATE_RUNNING,       /**< 运行中 */
    KT_TIMER_STATE_EXPIRED        /**< 已超时 */
} kt_timer_state_t;

/**
 * @brief 定时器模式
 */
typedef enum
{
    KT_TIMER_MODE_ONCE = 0,       /**< 单次模式 */
    KT_TIMER_MODE_PERIODIC        /**< 周期模式 */
} kt_timer_mode_t;

/**
 * @brief 定时器回调函数类型
 */
typedef void (*kt_timer_callback_t)(void* arg);

/**
 * @brief 软件定时器结构体
 */
typedef struct
{
    kt_uint32_t         id;             /**< 定时器ID */
    kt_timer_state_t    state;          /**< 状态 */
    kt_timer_mode_t     mode;           /**< 模式 */
    kt_tick_t           interval;       /**< 定时间隔（毫秒） */
    kt_tick_t           start_time;     /**< 启动时间 */
    kt_timer_callback_t callback;       /**< 回调函数 */
    void*               arg;            /**< 回调参数 */
    kt_bool_t           active;         /**< 是否激活 */
} kt_soft_timer_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化软件定时器模块
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_soft_timer_init(void);

/**
 * @brief 反初始化软件定时器模块
 */
void kt_soft_timer_deinit(void);

/**
 * @brief 创建定时器
 * @param timer 定时器指针
 * @param id 定时器ID
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_soft_timer_create(kt_soft_timer_t* timer, kt_uint32_t id);

/**
 * @brief 启动定时器
 * @param timer 定时器指针
 * @param interval 定时间隔（毫秒）
 * @param mode 定时器模式
 * @param callback 回调函数
 * @param arg 回调参数
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_soft_timer_start(kt_soft_timer_t* timer, kt_uint32_t interval,
                                kt_timer_mode_t mode, kt_timer_callback_t callback,
                                void* arg);

/**
 * @brief 停止定时器
 * @param timer 定时器指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_soft_timer_stop(kt_soft_timer_t* timer);

/**
 * @brief 重置定时器
 * @param timer 定时器指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_soft_timer_reset(kt_soft_timer_t* timer);

/**
 * @brief 检查定时器是否超时
 * @param timer 定时器指针
 * @return KT_TRUE 超时，KT_FALSE 未超时
 */
kt_bool_t kt_soft_timer_is_expired(kt_soft_timer_t* timer);

/**
 * @brief 获取定时器剩余时间
 * @param timer 定时器指针
 * @return 剩余时间（毫秒）
 */
kt_uint32_t kt_soft_timer_get_remaining(kt_soft_timer_t* timer);

/**
 * @brief 定时器心跳处理（需要在主循环中调用）
 */
void kt_soft_timer_tick(void);

/**
 * @brief 获取定时器状态
 * @param timer 定时器指针
 * @return 定时器状态
 */
kt_timer_state_t kt_soft_timer_get_state(kt_soft_timer_t* timer);

/**
 * @brief 判断定时器是否正在运行
 * @param timer 定时器指针
 * @return KT_TRUE 运行中，KT_FALSE 未运行
 */
kt_bool_t kt_soft_timer_is_running(kt_soft_timer_t* timer);

#endif /* KT_COMPONENT_SOFT_TIMER_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_SOFT_TIMER_H */