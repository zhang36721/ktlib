/**
 * @file kt_buzzer.h
 * @author kento
 * @brief 蜂鸣器驱动
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_BUZZER_H
#define KT_BUZZER_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_DRIVER_BUZZER_ENABLE

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief 蜂鸣器类型
 */
typedef enum
{
    KT_BUZZER_TYPE_PASSIVE = 0,     /**< 无源蜂鸣器 */
    KT_BUZZER_TYPE_ACTIVE           /**< 有源蜂鸣器 */
} kt_buzzer_type_t;

/**
 * @brief 蜂鸣器状态
 */
typedef enum
{
    KT_BUZZER_STATE_OFF = 0,        /**< 关闭 */
    KT_BUZZER_STATE_ON,             /**< 开启 */
    KT_BUZZER_STATE_TONE,           /**< 音调模式 */
    KT_BUZZER_STATE_MELODY          /**< 旋律模式 */
} kt_buzzer_state_t;

/**
 * @brief 蜂鸣器结构体
 */
typedef struct
{
    kt_uint32_t         id;             /**< 蜂鸣器ID */
    void*               port;           /**< GPIO端口 */
    kt_uint16_t         pin;            /**< GPIO引脚 */
    kt_buzzer_type_t    type;           /**< 蜂鸣器类型 */
    kt_buzzer_state_t   state;          /**< 当前状态 */
    kt_bool_t           is_on;          /**< 当前是否开启 */
    kt_uint32_t         frequency;      /**< 频率（音调模式） */
    kt_tick_t           start_time;     /**< 启动时间 */
    kt_uint32_t         duration;       /**< 持续时间 */
} kt_buzzer_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化蜂鸣器
 * @param buzzer 蜂鸣器指针
 * @param id 蜂鸣器ID
 * @param port GPIO端口
 * @param pin GPIO引脚
 * @param type 蜂鸣器类型
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_buzzer_init(kt_buzzer_t* buzzer, kt_uint32_t id, void* port, kt_uint16_t pin,
                           kt_buzzer_type_t type);

/**
 * @brief 打开蜂鸣器
 * @param buzzer 蜂鸣器指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_buzzer_on(kt_buzzer_t* buzzer);

/**
 * @brief 关闭蜂鸣器
 * @param buzzer 蜂鸣器指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_buzzer_off(kt_buzzer_t* buzzer);

/**
 * @brief 发出指定频率的声音（无源蜂鸣器）
 * @param buzzer 蜂鸣器指针
 * @param frequency 频率（Hz）
 * @param duration 持续时间（毫秒，0表示持续）
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_buzzer_tone(kt_buzzer_t* buzzer, kt_uint32_t frequency, kt_uint32_t duration);

/**
 * @brief 蜂鸣器心跳处理（需要在主循环中调用）
 * @param buzzer 蜂鸣器指针
 */
void kt_buzzer_tick(kt_buzzer_t* buzzer);

/**
 * @brief 获取蜂鸣器状态
 * @param buzzer 蜂鸣器指针
 * @return 蜂鸣器状态
 */
kt_buzzer_state_t kt_buzzer_get_state(kt_buzzer_t* buzzer);

#endif /* KT_DRIVER_BUZZER_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_BUZZER_H */