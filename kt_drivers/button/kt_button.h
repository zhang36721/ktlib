/**
 * @file kt_button.h
 * @author kento
 * @brief 按键驱动
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_BUTTON_H
#define KT_BUTTON_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_DRIVER_BUTTON_ENABLE

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief 按键事件
 */
typedef enum
{
    KT_BUTTON_EVENT_NONE = 0,       /**< 无事件 */
    KT_BUTTON_EVENT_PRESSED,        /**< 按下 */
    KT_BUTTON_EVENT_RELEASED,       /**< 释放 */
    KT_BUTTON_EVENT_SINGLE_CLICK,   /**< 单击 */
    KT_BUTTON_EVENT_DOUBLE_CLICK,   /**< 双击 */
    KT_BUTTON_EVENT_TRIPLE_CLICK,   /**< 三击 */
    KT_BUTTON_EVENT_LONG_PRESS,     /**< 长按 */
    KT_BUTTON_EVENT_LONG_PRESS_UP   /**< 长按释放 */
} kt_button_event_t;

/**
 * @brief 按键状态
 */
typedef enum
{
    KT_BUTTON_STATE_RELEASED = 0,   /**< 释放 */
    KT_BUTTON_STATE_PRESSED,        /**< 按下 */
    KT_BUTTON_STATE_DEBOUNCE        /**< 消抖中 */
} kt_button_state_t;

/**
 * @brief 按键事件回调函数类型
 */
typedef void (*kt_button_callback_t)(void* arg, kt_button_event_t event);

/**
 * @brief 按键结构体
 */
typedef struct
{
    kt_uint32_t             id;             /**< 按键ID */
    void*                   port;           /**< GPIO端口 */
    kt_uint16_t             pin;            /**< GPIO引脚 */
    kt_button_state_t       state;          /**< 当前状态 */
    kt_bool_t               active_level;   /**< 有效电平（0=低电平有效，1=高电平有效） */
    kt_tick_t               press_time;     /**< 按下时间 */
    kt_tick_t               release_time;   /**< 释放时间 */
    kt_uint32_t             click_count;    /**< 连续点击次数 */
    kt_tick_t               last_click;     /**< 上次点击时间 */
    kt_button_callback_t    callback;       /**< 事件回调 */
    void*                   arg;            /**< 回调参数 */
    kt_bool_t               long_pressed;   /**< 是否已触发长按 */
} kt_button_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化按键
 * @param btn 按键指针
 * @param id 按键ID
 * @param port GPIO端口
 * @param pin GPIO引脚
 * @param active_level 有效电平（0=低电平有效，1=高电平有效）
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_button_init(kt_button_t* btn, kt_uint32_t id, void* port, kt_uint16_t pin,
                           kt_bool_t active_level);

/**
 * @brief 设置按键事件回调
 * @param btn 按键指针
 * @param callback 回调函数
 * @param arg 回调参数
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_button_set_callback(kt_button_t* btn, kt_button_callback_t callback, void* arg);

/**
 * @brief 按键扫描处理（需要在主循环中定期调用）
 * @param btn 按键指针
 * @return 按键事件
 */
kt_button_event_t kt_button_scan(kt_button_t* btn);

/**
 * @brief 获取按键状态
 * @param btn 按键指针
 * @return 按键状态
 */
kt_button_state_t kt_button_get_state(kt_button_t* btn);

/**
 * @brief 检查按键是否被按下
 * @param btn 按键指针
 * @return KT_TRUE 按下，KT_FALSE 未按下
 */
kt_bool_t kt_button_is_pressed(kt_button_t* btn);

/**
 * @brief 重置按键状态
 * @param btn 按键指针
 */
void kt_button_reset(kt_button_t* btn);

#endif /* KT_DRIVER_BUTTON_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_BUTTON_H */