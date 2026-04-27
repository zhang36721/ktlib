/**
 * @file kt_port_gpio_esp32.c
 * @author kento
 * @brief ESP32 GPIO适配实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_ESP32

#include "driver/gpio.h"

/**
 * @brief 初始化GPIO引脚
 */
kt_status_t kt_port_gpio_init(void* port, kt_uint16_t pin, 
                              kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    gpio_config_t io_conf = {0};
    
    /* 检查参数 */
    if (pin > 39)  /* ESP32 GPIO最大编号 */
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    io_conf.pin_bit_mask = (1ULL << pin);
    
    /* 设置模式 */
    switch (mode)
    {
        case KT_GPIO_MODE_OUTPUT:
            io_conf.mode = GPIO_MODE_OUTPUT;
            break;
            
        case KT_GPIO_MODE_INPUT:
            io_conf.mode = GPIO_MODE_INPUT;
            break;
            
        case KT_GPIO_MODE_ANALOG:
            /* ESP32的ADC功能通过其他API配置 */
            return KT_ERR_NOT_SUPPORTED;
            
        case KT_GPIO_MODE_AF:
            io_conf.mode = GPIO_MODE_OUTPUT;
            break;
            
        default:
            return KT_ERR_INVALID_PARAM;
    }
    
    /* 设置上下拉 */
    switch (pull)
    {
        case KT_GPIO_PULL_UP:
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
            
        case KT_GPIO_PULL_DOWN:
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
            
        case KT_GPIO_PULL_NONE:
        default:
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
    }
    
    io_conf.intr_type = GPIO_INTR_DISABLE;
    
    gpio_config(&io_conf);
    
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚
 */
void kt_port_gpio_write(void* port, kt_uint16_t pin, kt_uint8_t value)
{
    if (pin > 39)
    {
        return;
    }
    
    gpio_set_level((gpio_num_t)pin, value);
}

/**
 * @brief 读取GPIO引脚
 */
kt_uint8_t kt_port_gpio_read(void* port, kt_uint16_t pin)
{
    if (pin > 39)
    {
        return 0;
    }
    
    return (kt_uint8_t)gpio_get_level((gpio_num_t)pin);
}

/**
 * @brief 切换GPIO引脚状态
 */
void kt_port_gpio_toggle(void* port, kt_uint16_t pin)
{
    if (pin > 39)
    {
        return;
    }
    
    uint8_t current = gpio_get_level((gpio_num_t)pin);
    gpio_set_level((gpio_num_t)pin, !current);
}

#endif /* USE_KT_PLATFORM_ESP32 */