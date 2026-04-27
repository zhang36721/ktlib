/**
 * @file kt_buzzer.c
 * @author kento
 * @brief 蜂鸣器驱动实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_buzzer.h"
#include "kt_port.h"

#if KT_DRIVER_BUZZER_ENABLE

/**
 * @brief 初始化蜂鸣器
 */
kt_status_t kt_buzzer_init(kt_buzzer_t* buzzer, kt_uint32_t id, void* port, kt_uint16_t pin,
                           kt_buzzer_type_t type)
{
    if (buzzer == NULL || port == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 初始化GPIO为输出模式 */
    kt_port_gpio_init(port, pin, KT_GPIO_MODE_OUTPUT, KT_GPIO_PULL_NONE);
    
    buzzer->id = id;
    buzzer->port = port;
    buzzer->pin = pin;
    buzzer->type = type;
    buzzer->state = KT_BUZZER_STATE_OFF;
    buzzer->is_on = KT_FALSE;
    buzzer->frequency = 0;
    buzzer->start_time = 0;
    buzzer->duration = 0;
    
    /* 默认关闭蜂鸣器 */
    kt_port_gpio_write(port, pin, 0);
    
    return KT_OK;
}

/**
 * @brief 打开蜂鸣器
 */
kt_status_t kt_buzzer_on(kt_buzzer_t* buzzer)
{
    if (buzzer == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    buzzer->state = KT_BUZZER_STATE_ON;
    buzzer->is_on = KT_TRUE;
    kt_port_gpio_write(buzzer->port, buzzer->pin, 1);
    
    return KT_OK;
}

/**
 * @brief 关闭蜂鸣器
 */
kt_status_t kt_buzzer_off(kt_buzzer_t* buzzer)
{
    if (buzzer == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    buzzer->state = KT_BUZZER_STATE_OFF;
    buzzer->is_on = KT_FALSE;
    kt_port_gpio_write(buzzer->port, buzzer->pin, 0);
    
    return KT_OK;
}

/**
 * @brief 发出指定频率的声音
 */
kt_status_t kt_buzzer_tone(kt_buzzer_t* buzzer, kt_uint32_t frequency, kt_uint32_t duration)
{
    if (buzzer == NULL || frequency == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 只有无源蜂鸣器支持音调模式 */
    if (buzzer->type != KT_BUZZER_TYPE_PASSIVE)
    {
        return KT_ERR_NOT_SUPPORTED;
    }
    
    buzzer->state = KT_BUZZER_STATE_TONE;
    buzzer->frequency = frequency;
    buzzer->duration = duration;
    buzzer->start_time = kt_port_get_tick();
    buzzer->is_on = KT_TRUE;
    
    /* 注意：实际频率控制需要定时器/PWM硬件支持 */
    /* 这里只是简单开关，实际应用中需要实现PWM */
    kt_port_gpio_write(buzzer->port, buzzer->pin, 1);
    
    return KT_OK;
}

/**
 * @brief 蜂鸣器心跳处理
 */
void kt_buzzer_tick(kt_buzzer_t* buzzer)
{
    kt_tick_t current_time;
    kt_uint32_t elapsed;
    
    if (buzzer == NULL)
    {
        return;
    }
    
    if (buzzer->state != KT_BUZZER_STATE_TONE)
    {
        return;
    }
    
    if (buzzer->duration == 0)
    {
        return;  /* 持续发声 */
    }
    
    current_time = kt_port_get_tick();
    elapsed = current_time - buzzer->start_time;
    
    if (elapsed >= buzzer->duration)
    {
        /* 持续时间到，关闭蜂鸣器 */
        kt_buzzer_off(buzzer);
    }
}

/**
 * @brief 获取蜂鸣器状态
 */
kt_buzzer_state_t kt_buzzer_get_state(kt_buzzer_t* buzzer)
{
    if (buzzer == NULL)
    {
        return KT_BUZZER_STATE_OFF;
    }
    
    return buzzer->state;
}

#endif /* KT_DRIVER_BUZZER_ENABLE */