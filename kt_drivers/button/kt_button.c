/**
 * @file kt_button.c
 * @author kento
 * @brief 按键驱动实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_button.h"
#include "kt_port.h"

#if KT_DRIVER_BUTTON_ENABLE

/** 消抖时间（毫秒） */
#define BUTTON_DEBOUNCE_MS      20

/**
 * @brief 初始化按键
 */
kt_status_t kt_button_init(kt_button_t* btn, kt_uint32_t id, void* port, kt_uint16_t pin,
                           kt_bool_t active_level)
{
    if (btn == NULL || port == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 初始化GPIO为输入模式 */
    kt_port_gpio_init(port, pin, KT_GPIO_MODE_INPUT, 
                      active_level ? KT_GPIO_PULL_DOWN : KT_GPIO_PULL_UP);
    
    btn->id = id;
    btn->port = port;
    btn->pin = pin;
    btn->state = KT_BUTTON_STATE_RELEASED;
    btn->active_level = active_level;
    btn->press_time = 0;
    btn->release_time = 0;
    btn->click_count = 0;
    btn->last_click = 0;
    btn->callback = NULL;
    btn->arg = NULL;
    btn->long_pressed = KT_FALSE;
    
    return KT_OK;
}

/**
 * @brief 设置按键事件回调
 */
kt_status_t kt_button_set_callback(kt_button_t* btn, kt_button_callback_t callback, void* arg)
{
    if (btn == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    btn->callback = callback;
    btn->arg = arg;
    
    return KT_OK;
}

/**
 * @brief 读取按键原始状态
 */
static kt_bool_t button_read_raw(kt_button_t* btn)
{
    kt_uint8_t level = kt_port_gpio_read(btn->port, btn->pin);
    
    /* 根据有效电平返回状态 */
    if (btn->active_level)
    {
        return (level == 1) ? KT_TRUE : KT_FALSE;
    }
    else
    {
        return (level == 0) ? KT_TRUE : KT_FALSE;
    }
}

/**
 * @brief 触发回调
 */
static void button_trigger_event(kt_button_t* btn, kt_button_event_t event)
{
    if (btn->callback != NULL)
    {
        btn->callback(btn->arg, event);
    }
}

/**
 * @brief 按键扫描处理
 */
kt_button_event_t kt_button_scan(kt_button_t* btn)
{
    kt_bool_t raw_state;
    kt_tick_t current_time;
    kt_uint32_t press_duration;
    kt_button_event_t event = KT_BUTTON_EVENT_NONE;
    
    if (btn == NULL)
    {
        return KT_BUTTON_EVENT_NONE;
    }
    
    raw_state = button_read_raw(btn);
    current_time = kt_port_get_tick();
    
    switch (btn->state)
    {
        case KT_BUTTON_STATE_RELEASED:
            if (raw_state)
            {
                /* 检测到按下，进入消抖 */
                btn->state = KT_BUTTON_STATE_DEBOUNCE;
                btn->press_time = current_time;
            }
            break;
            
        case KT_BUTTON_STATE_DEBOUNCE:
            if (raw_state)
            {
                /* 消抖后确认按下 */
                if (current_time - btn->press_time >= BUTTON_DEBOUNCE_MS)
                {
                    btn->state = KT_BUTTON_STATE_PRESSED;
                    event = KT_BUTTON_EVENT_PRESSED;
                    button_trigger_event(btn, event);
                }
            }
            else
            {
                /* 消抖期间恢复释放 */
                btn->state = KT_BUTTON_STATE_RELEASED;
            }
            break;
            
        case KT_BUTTON_STATE_PRESSED:
            if (!raw_state)
            {
                /* 检测到释放 */
                btn->release_time = current_time;
                press_duration = btn->release_time - btn->press_time;
                btn->state = KT_BUTTON_STATE_RELEASED;
                
                /* 判断长按 */
                if (press_duration >= KT_BUTTON_LONG_PRESS_MS)
                {
                    if (!btn->long_pressed)
                    {
                        event = KT_BUTTON_EVENT_LONG_PRESS;
                        button_trigger_event(btn, event);
                        btn->long_pressed = KT_TRUE;
                    }
                    event = KT_BUTTON_EVENT_LONG_PRESS_UP;
                    button_trigger_event(btn, event);
                }
                else
                {
                    /* 判断点击次数 */
                    if (btn->last_click > 0 && 
                        (current_time - btn->last_click) < KT_BUTTON_DOUBLE_CLICK_MS)
                    {
                        btn->click_count++;
                    }
                    else
                    {
                        btn->click_count = 1;
                    }
                    
                    btn->last_click = current_time;
                    
                    /* 根据点击次数触发事件 */
                    if (btn->click_count == 1)
                    {
                        event = KT_BUTTON_EVENT_SINGLE_CLICK;
                        button_trigger_event(btn, event);
                    }
                    else if (btn->click_count == 2)
                    {
                        event = KT_BUTTON_EVENT_DOUBLE_CLICK;
                        button_trigger_event(btn, event);
                    }
                    else if (btn->click_count >= 3)
                    {
                        event = KT_BUTTON_EVENT_TRIPLE_CLICK;
                        button_trigger_event(btn, event);
                        btn->click_count = 0;  /* 重置计数 */
                    }
                }
                
                event = KT_BUTTON_EVENT_RELEASED;
                button_trigger_event(btn, event);
                btn->long_pressed = KT_FALSE;
            }
            else
            {
                /* 检查长按 */
                if (!btn->long_pressed)
                {
                    press_duration = current_time - btn->press_time;
                    if (press_duration >= KT_BUTTON_LONG_PRESS_MS)
                    {
                        event = KT_BUTTON_EVENT_LONG_PRESS;
                        button_trigger_event(btn, event);
                        btn->long_pressed = KT_TRUE;
                    }
                }
            }
            break;
            
        default:
            btn->state = KT_BUTTON_STATE_RELEASED;
            break;
    }
    
    return event;
}

/**
 * @brief 获取按键状态
 */
kt_button_state_t kt_button_get_state(kt_button_t* btn)
{
    if (btn == NULL)
    {
        return KT_BUTTON_STATE_RELEASED;
    }
    
    return btn->state;
}

/**
 * @brief 检查按键是否被按下
 */
kt_bool_t kt_button_is_pressed(kt_button_t* btn)
{
    if (btn == NULL)
    {
        return KT_FALSE;
    }
    
    return (btn->state == KT_BUTTON_STATE_PRESSED) ? KT_TRUE : KT_FALSE;
}

/**
 * @brief 重置按键状态
 */
void kt_button_reset(kt_button_t* btn)
{
    if (btn == NULL)
    {
        return;
    }
    
    btn->state = KT_BUTTON_STATE_RELEASED;
    btn->press_time = 0;
    btn->release_time = 0;
    btn->click_count = 0;
    btn->last_click = 0;
    btn->long_pressed = KT_FALSE;
}

#endif /* KT_DRIVER_BUTTON_ENABLE */