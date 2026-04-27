/**
 * @file kt_led.c
 * @author kento
 * @brief LED驱动实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_led.h"
#include "kt_port.h"

#if KT_DRIVER_LED_ENABLE

/**
 * @brief 默认LED控制函数
 */
static void default_led_control(void* port, kt_uint16_t pin, kt_uint8_t value)
{
    kt_port_gpio_write(port, pin, value);
}

/**
 * @brief 初始化LED
 */
kt_status_t kt_led_init(kt_led_t* led, kt_uint32_t id, void* port, kt_uint16_t pin,
                        kt_led_control_t control)
{
    if (led == NULL || port == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 初始化GPIO */
    kt_port_gpio_init(port, pin, KT_GPIO_MODE_OUTPUT, KT_GPIO_PULL_NONE);
    
    led->id = id;
    led->port = port;
    led->pin = pin;
    led->state = KT_LED_STATE_OFF;
    led->control = (control != NULL) ? control : default_led_control;
    led->on_time = 0;
    led->off_time = 0;
    led->last_toggle = 0;
    led->is_on = KT_FALSE;
    led->brightness = 0;
    
    /* 默认关闭LED */
    led->control(port, pin, 0);
    
    return KT_OK;
}

/**
 * @brief 打开LED
 */
kt_status_t kt_led_on(kt_led_t* led)
{
    if (led == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    led->state = KT_LED_STATE_ON;
    led->is_on = KT_TRUE;
    led->control(led->port, led->pin, 1);
    
    return KT_OK;
}

/**
 * @brief 关闭LED
 */
kt_status_t kt_led_off(kt_led_t* led)
{
    if (led == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    led->state = KT_LED_STATE_OFF;
    led->is_on = KT_FALSE;
    led->control(led->port, led->pin, 0);
    
    return KT_OK;
}

/**
 * @brief 切换LED状态
 */
kt_status_t kt_led_toggle(kt_led_t* led)
{
    if (led == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    if (led->is_on)
    {
        return kt_led_off(led);
    }
    else
    {
        return kt_led_on(led);
    }
}

/**
 * @brief 设置LED闪烁
 */
kt_status_t kt_led_blink(kt_led_t* led, kt_uint32_t on_time, kt_uint32_t off_time)
{
    if (led == NULL || on_time == 0 || off_time == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    led->state = KT_LED_STATE_BLINK;
    led->on_time = on_time;
    led->off_time = off_time;
    led->last_toggle = kt_port_get_tick();
    
    /* 先打开LED */
    led->is_on = KT_TRUE;
    led->control(led->port, led->pin, 1);
    
    return KT_OK;
}

/**
 * @brief 设置LED呼吸效果
 */
kt_status_t kt_led_breath(kt_led_t* led, kt_uint32_t speed)
{
    if (led == NULL || speed == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 呼吸效果需要PWM支持，这里只做简单模拟 */
    led->state = KT_LED_STATE_BREATH;
    led->brightness = 0;
    
    /* 注意：完整的呼吸效果需要PWM硬件支持 */
    /* 这里只是设置状态，实际效果取决于控制函数 */
    
    return KT_OK;
}

/**
 * @brief LED心跳处理
 */
void kt_led_tick(kt_led_t* led)
{
    kt_tick_t current_time;
    kt_uint32_t elapsed;
    
    if (led == NULL)
    {
        return;
    }
    
    if (led->state != KT_LED_STATE_BLINK)
    {
        return;
    }
    
    current_time = kt_port_get_tick();
    elapsed = current_time - led->last_toggle;
    
    if (led->is_on && elapsed >= led->on_time)
    {
        /* 从开启切换到关闭 */
        led->is_on = KT_FALSE;
        led->control(led->port, led->pin, 0);
        led->last_toggle = current_time;
    }
    else if (!led->is_on && elapsed >= led->off_time)
    {
        /* 从关闭切换到开启 */
        led->is_on = KT_TRUE;
        led->control(led->port, led->pin, 1);
        led->last_toggle = current_time;
    }
}

/**
 * @brief 获取LED状态
 */
kt_led_state_t kt_led_get_state(kt_led_t* led)
{
    if (led == NULL)
    {
        return KT_LED_STATE_OFF;
    }
    
    return led->state;
}

/**
 * @brief 停止LED效果
 */
kt_status_t kt_led_stop(kt_led_t* led)
{
    if (led == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    led->state = KT_LED_STATE_OFF;
    led->is_on = KT_FALSE;
    led->control(led->port, led->pin, 0);
    
    return KT_OK;
}

#endif /* KT_DRIVER_LED_ENABLE */