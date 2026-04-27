/**
 * @file kt_port.h
 * @author kento
 * @brief 平台适配层接口声明
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 * 
 * @attention 本文件定义平台适配层的统一接口，并包含具体平台的实现
 */

#ifndef KT_PORT_H
#define KT_PORT_H

#include "kt_types.h"
#include "kt_config.h"
#include "kt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                         GPIO 相关定义
 *============================================================================*/

/**
 * @brief GPIO模式
 */
typedef enum
{
    KT_GPIO_MODE_INPUT,     /**< 输入模式 */
    KT_GPIO_MODE_OUTPUT,    /**< 输出模式 */
    KT_GPIO_MODE_ANALOG,    /**< 模拟模式 */
    KT_GPIO_MODE_AF         /**< 复用功能模式 */
} kt_gpio_mode_t;

/**
 * @brief GPIO上下拉配置
 */
typedef enum
{
    KT_GPIO_PULL_NONE,      /**< 无上拉下拉 */
    KT_GPIO_PULL_UP,        /**< 上拉 */
    KT_GPIO_PULL_DOWN       /**< 下拉 */
} kt_gpio_pull_t;

/**
 * @brief GPIO引脚状态
 */
typedef enum
{
    KT_GPIO_STATE_LOW = 0,  /**< 低电平 */
    KT_GPIO_STATE_HIGH = 1  /**< 高电平 */
} kt_gpio_state_t;

/*============================================================================
 *                         UART 相关定义
 *============================================================================*/

/**
 * @brief UART数据位
 */
typedef enum
{
    KT_UART_DATA_5 = 5,
    KT_UART_DATA_6 = 6,
    KT_UART_DATA_7 = 7,
    KT_UART_DATA_8 = 8
} kt_uart_data_t;

/**
 * @brief UART停止位
 */
typedef enum
{
    KT_UART_STOP_1,         /**< 1位停止位 */
    KT_UART_STOP_2          /**< 2位停止位 */
} kt_uart_stop_t;

/**
 * @brief UART校验位
 */
typedef enum
{
    KT_UART_PARITY_NONE,    /**< 无校验 */
    KT_UART_PARITY_EVEN,    /**< 偶校验 */
    KT_UART_PARITY_ODD      /**< 奇校验 */
} kt_uart_parity_t;

/*============================================================================
 *                         GPIO 接口声明
 *============================================================================*/

/**
 * @brief 初始化GPIO引脚
 * @param port GPIO端口（平台相关）
 * @param pin GPIO引脚号
 * @param mode GPIO模式
 * @param pull 上下拉配置
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_port_gpio_init(void* port, kt_uint16_t pin, 
                              kt_gpio_mode_t mode, kt_gpio_pull_t pull);

/**
 * @brief 写入GPIO引脚
 * @param port GPIO端口
 * @param pin GPIO引脚号
 * @param value 写入值（0或1）
 */
void kt_port_gpio_write(void* port, kt_uint16_t pin, kt_uint8_t value);

/**
 * @brief 读取GPIO引脚
 * @param port GPIO端口
 * @param pin GPIO引脚号
 * @return 读取值（0或1）
 */
kt_uint8_t kt_port_gpio_read(void* port, kt_uint16_t pin);

/**
 * @brief 切换GPIO引脚状态
 * @param port GPIO端口
 * @param pin GPIO引脚号
 */
void kt_port_gpio_toggle(void* port, kt_uint16_t pin);

/*============================================================================
 *                         UART 接口声明
 *============================================================================*/

/**
 * @brief 初始化UART
 * @param uart_handle UART句柄（平台相关）
 * @param baudrate 波特率
 * @param data_bits 数据位
 * @param stop_bits 停止位
 * @param parity 校验位
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_port_uart_init(void* uart_handle, kt_uint32_t baudrate,
                              kt_uart_data_t data_bits, kt_uart_stop_t stop_bits,
                              kt_uart_parity_t parity);

/**
 * @brief 通过UART发送数据（阻塞）
 * @param uart_handle UART句柄
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 实际发送的字节数
 */
kt_int32_t kt_port_uart_send(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length);

/**
 * @brief 通过UART接收数据（阻塞）
 * @param uart_handle UART句柄
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 实际接收的字节数
 */
kt_int32_t kt_port_uart_receive(void* uart_handle, kt_uint8_t* data, kt_uint16_t length);

/**
 * @brief 通过UART发送数据（非阻塞）
 * @param uart_handle UART句柄
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_port_uart_send_nb(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length);

/**
 * @brief 通过UART接收数据（非阻塞）
 * @param uart_handle UART句柄
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 实际接收的字节数
 */
kt_int32_t kt_port_uart_receive_nb(void* uart_handle, kt_uint8_t* data, kt_uint16_t length);

/*============================================================================
 *                         时间 接口声明
 *============================================================================*/

/**
 * @brief 毫秒级延时
 * @param ms 延时毫秒数
 */
void kt_port_delay_ms(kt_uint32_t ms);

/**
 * @brief 微秒级延时
 * @param us 延时微秒数
 */
void kt_port_delay_us(kt_uint32_t us);

/**
 * @brief 获取系统时钟节拍（毫秒）
 * @return 系统时钟节拍值
 */
kt_tick_t kt_port_get_tick(void);

/**
 * @brief 获取系统时钟频率（Hz）
 * @return 系统时钟频率
 */
kt_uint32_t kt_port_get_tick_freq(void);

/*============================================================================
 *                         平台特定包含
 *============================================================================*/

#if defined(USE_KT_PLATFORM_STM32_HAL)
    #include "stm32_hal/kt_port_stm32_hal.h"
#elif defined(USE_KT_PLATFORM_ESP32)
    #include "esp32/kt_port_esp32.h"
#elif defined(USE_KT_PLATFORM_8051)
    #include "8051/kt_port_8051.h"
#else
    #include "mock/kt_port_mock.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* KT_PORT_H */