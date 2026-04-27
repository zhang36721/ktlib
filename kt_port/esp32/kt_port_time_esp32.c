/**
 * @file kt_port_time_esp32.c
 * @author kento
 * @brief ESP32 时间适配实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_ESP32

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

/**
 * @brief 毫秒级延时
 */
void kt_port_delay_ms(kt_uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief 微秒级延时
 * @note 使用esp_timer实现微秒级延时
 */
void kt_port_delay_us(kt_uint32_t us)
{
    esp_timer_handle_t one_shot_timer;
    const esp_timer_create_args_t timer_args = {
        .callback = NULL,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "delay_us",
        .skip_unhandled_events = false,
    };
    
    esp_timer_create(&timer_args, &one_shot_timer);
    esp_timer_start_once(one_shot_timer, us);
    
    /* 等待延时结束 */
    while (esp_timer_get_expiry_time(one_shot_timer) != 0)
    {
        /* 空循环等待 */
    }
    
    esp_timer_delete(one_shot_timer);
}

/**
 * @brief 获取系统时钟节拍（毫秒）
 */
kt_tick_t kt_port_get_tick(void)
{
    return (kt_tick_t)(esp_timer_get_time() / 1000);
}

/**
 * @brief 获取系统时钟频率（Hz）
 */
kt_uint32_t kt_port_get_tick_freq(void)
{
    /* 返回1000，表示tick频率为1kHz（1ms） */
    return 1000U;
}

#endif /* USE_KT_PLATFORM_ESP32 */