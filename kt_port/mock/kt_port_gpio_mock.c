/**
 * @file kt_port_gpio_mock.c
 * @author kento
 * @brief Mock平台 GPIO模拟实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_MOCK

#include <stdio.h>
#include <stdlib.h>

/* 模拟GPIO状态存储 */
#define MOCK_GPIO_MAX_PINS    64

typedef struct
{
    kt_uint8_t  state;      /* 引脚状态 (0/1) */
    kt_gpio_mode_t mode;    /* 引脚模式 */
    kt_gpio_pull_t pull;    /* 上下拉配置 */
    kt_bool_t   initialized;
} mock_gpio_pin_t;

static mock_gpio_pin_t mock_gpio_pins[MOCK_GPIO_MAX_PINS] = {0};

/**
 * @brief 初始化GPIO引脚（模拟）
 */
kt_status_t kt_port_gpio_init(void* port, kt_uint16_t pin, 
                              kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    if (pin >= MOCK_GPIO_MAX_PINS)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    mock_gpio_pins[pin].state = 0;
    mock_gpio_pins[pin].mode = mode;
    mock_gpio_pins[pin].pull = pull;
    mock_gpio_pins[pin].initialized = KT_TRUE;
    
    printf("[MOCK] GPIO init: port=%p, pin=%d, mode=%d, pull=%d\n", 
           port, pin, mode, pull);
    
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚（模拟）
 */
void kt_port_gpio_write(void* port, kt_uint16_t pin, kt_uint8_t value)
{
    if (pin >= MOCK_GPIO_MAX_PINS || !mock_gpio_pins[pin].initialized)
    {
        return;
    }
    
    mock_gpio_pins[pin].state = value ? 1 : 0;
    
    printf("[MOCK] GPIO write: port=%p, pin=%d, value=%d\n", port, pin, value);
}

/**
 * @brief 读取GPIO引脚（模拟）
 */
kt_uint8_t kt_port_gpio_read(void* port, kt_uint16_t pin)
{
    if (pin >= MOCK_GPIO_MAX_PINS || !mock_gpio_pins[pin].initialized)
    {
        return 0;
    }
    
    printf("[MOCK] GPIO read: port=%p, pin=%d\n", port, pin);
    
    return mock_gpio_pins[pin].state;
}

/**
 * @brief 切换GPIO引脚状态（模拟）
 */
void kt_port_gpio_toggle(void* port, kt_uint16_t pin)
{
    if (pin >= MOCK_GPIO_MAX_PINS || !mock_gpio_pins[pin].initialized)
    {
        return;
    }
    
    mock_gpio_pins[pin].state = !mock_gpio_pins[pin].state;
    
    printf("[MOCK] GPIO toggle: port=%p, pin=%d, new_state=%d\n", 
           port, pin, mock_gpio_pins[pin].state);
}

#endif /* USE_KT_PLATFORM_MOCK */