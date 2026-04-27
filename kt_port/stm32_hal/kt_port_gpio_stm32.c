/**
 * @file kt_port_gpio_stm32.c
 * @author kento
 * @brief STM32 HAL库 GPIO适配实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_STM32_HAL

/* 包含STM32 HAL头文件，根据具体型号调整 */
#if defined(STM32F103xB) || defined(STM32F103xE)
    #include "stm32f1xx_hal.h"
#elif defined(STM32F401xE) || defined(STM32F401xC) || defined(STM32F411xE)
    #include "stm32f4xx_hal.h"
#elif defined(STM32L476xx) || defined(STM32L433xx)
    #include "stm32l4xx_hal.h"
#else
    #include "stm32f4xx_hal.h"  /* 默认使用STM32F4 */
#endif

/**
 * @brief 初始化GPIO引脚
 */
kt_status_t kt_port_gpio_init(void* port, kt_uint16_t pin, 
                              kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    
    /* 检查参数 */
    if (gpio_port == NULL || pin > 15)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* 配置GPIO参数 */
    GPIO_InitStruct.Pin = (uint16_t)(1U << pin);
    
    /* 设置模式 */
    switch (mode)
    {
        case KT_GPIO_MODE_OUTPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            break;
            
        case KT_GPIO_MODE_INPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            break;
            
        case KT_GPIO_MODE_ANALOG:
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            break;
            
        case KT_GPIO_MODE_AF:
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            break;
            
        default:
            return KT_ERR_INVALID_PARAM;
    }
    
    /* 设置上下拉 */
    switch (pull)
    {
        case KT_GPIO_PULL_UP:
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
            
        case KT_GPIO_PULL_DOWN:
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
            
        case KT_GPIO_PULL_NONE:
        default:
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
    }
    
    /* 初始化GPIO */
    HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
    
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚
 */
void kt_port_gpio_write(void* port, kt_uint16_t pin, kt_uint8_t value)
{
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    
    if (gpio_port == NULL || pin > 15)
    {
        return;
    }
    
    HAL_GPIO_WritePin(gpio_port, (uint16_t)(1U << pin), 
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief 读取GPIO引脚
 */
kt_uint8_t kt_port_gpio_read(void* port, kt_uint16_t pin)
{
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    
    if (gpio_port == NULL || pin > 15)
    {
        return 0;
    }
    
    return (kt_uint8_t)(HAL_GPIO_ReadPin(gpio_port, (uint16_t)(1U << pin)) == GPIO_PIN_SET ? 1 : 0);
}

/**
 * @brief 切换GPIO引脚状态
 */
void kt_port_gpio_toggle(void* port, kt_uint16_t pin)
{
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    
    if (gpio_port == NULL || pin > 15)
    {
        return;
    }
    
    HAL_GPIO_TogglePin(gpio_port, (uint16_t)(1U << pin));
}

#endif /* USE_KT_PLATFORM_STM32_HAL */