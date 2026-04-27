/**
 * @file kt_port_time_stm32.c
 * @author kento
 * @brief STM32 HAL库 时间适配实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_STM32_HAL

/* 包含STM32 HAL头文件 */
#if defined(STM32F103xB) || defined(STM32F103xE)
    #include "stm32f1xx_hal.h"
#elif defined(STM32F401xE) || defined(STM32F401xC) || defined(STM32F411xE)
    #include "stm32f4xx_hal.h"
#elif defined(STM32L476xx) || defined(STM32L433xx)
    #include "stm32l4xx_hal.h"
#else
    #include "stm32f4xx_hal.h"
#endif

/**
 * @brief 毫秒级延时
 */
void kt_port_delay_ms(kt_uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief 微秒级延时
 * @note 使用HAL_Delay_us（如果可用）或简单循环实现
 */
void kt_port_delay_us(kt_uint32_t us)
{
#if defined(STM32F4xx) || defined(STM32L4xx)
    /* STM32F4/L4系列支持HAL_Delay_us */
    HAL_Delay(us);
#else
    /* 其他系列使用简单循环延时 */
    /* 需要根据系统时钟频率调整循环次数 */
    volatile uint32_t count;
    while (us--)
    {
        /* 假设系统时钟为72MHz，每次循环约1us */
        count = 18;
        while (count--);
    }
#endif
}

/**
 * @brief 获取系统时钟节拍（毫秒）
 */
kt_tick_t kt_port_get_tick(void)
{
    return (kt_tick_t)HAL_GetTick();
}

/**
 * @brief 获取系统时钟频率（Hz）
 */
kt_uint32_t kt_port_get_tick_freq(void)
{
    /* 返回1000，表示tick频率为1kHz（1ms） */
    return 1000U;
}

#endif /* USE_KT_PLATFORM_STM32_HAL */