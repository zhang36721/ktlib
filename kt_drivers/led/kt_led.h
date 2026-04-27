/**
 * @file kt_led.h
 * @author kento
 * @brief LED驱动
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#ifndef KT_LED_H
#define KT_LED_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if KT_DRIVER_LED_ENABLE

/*============================================================================
 *                         结构体定义
 *============================================================================*/

/**
 * @brief LED状态
 */
typedef enum
{
    KT_LED_STATE_OFF = 0,       /**< 关闭 */
    KT_LED_STATE_ON,            /**< 开启 */
    KT_LED_STATE_BLINK,         /**< 闪烁 */
    KT_LED_STATE_BREATH,        /**< 呼吸 */
    KT_LED_STATE_CUSTOM         /**< 自定义模式 */
} kt_led_state_t;

/**
 * @brief LED控制函数类型
 */
typedef void (*kt_led_control_t)(void* port, kt_uint16_t pin, kt_uint8_t value);

/**
 * @brief LED结构体
 */
typedef struct
{
    kt_uint32_t         id;             /**< LED ID */
    void*               port;           /**< GPIO端口 */
    kt_uint16_t         pin;            /**< GPIO引脚 */
    kt_led_state_t      state;          /**< 当前状态 */
    kt_led_control_t    control;        /**< 控制函数 */
    kt_uint32_t         on_time;        /**< 开启时间（闪烁模式） */
    kt_uint32_t         off_time;       /**< 关闭时间（闪烁模式） */
    kt_tick_t           last_toggle;    /**< 上次切换时间 */
    kt_bool_t           is_on;          /**< 当前是否亮 */
    kt_uint8_t          brightness;     /**< 亮度（呼吸模式） */
} kt_led_t;

/*============================================================================
 *                         函数声明
 *============================================================================*/

/**
 * @brief 初始化LED
 * @param led LED指针
 * @param id LED ID
 * @param port GPIO端口
 * @param pin GPIO引脚
 * @param control 控制函数
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_init(kt_led_t* led, kt_uint32_t id, void* port, kt_uint16_t pin,
                        kt_led_control_t control);

/**
 * @brief 打开LED
 * @param led LED指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_on(kt_led_t* led);

/**
 * @brief 关闭LED
 * @param led LED指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_off(kt_led_t* led);

/**
 * @brief 切换LED状态
 * @param led LED指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_toggle(kt_led_t* led);

/**
 * @brief 设置LED闪烁
 * @param led LED指针
 * @param on_time 开启时间（毫秒）
 * @param off_time 关闭时间（毫秒）
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_blink(kt_led_t* led, kt_uint32_t on_time, kt_uint32_t off_time);

/**
 * @brief 设置LED呼吸效果
 * @param led LED指针
 * @param speed 呼吸速度
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_breath(kt_led_t* led, kt_uint32_t speed);

/**
 * @brief LED心跳处理（需要在主循环中调用）
 * @param led LED指针
 */
void kt_led_tick(kt_led_t* led);

/**
 * @brief 获取LED状态
 * @param led LED指针
 * @return LED状态
 */
kt_led_state_t kt_led_get_state(kt_led_t* led);

/**
 * @brief 停止LED效果
 * @param led LED指针
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_led_stop(kt_led_t* led);

#endif /* KT_DRIVER_LED_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* KT_LED_H */