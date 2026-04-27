/**
 * @file kt_port_uart_stm32.c
 * @author kento
 * @brief STM32 HAL库 UART适配实现
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
 * @brief 初始化UART
 */
kt_status_t kt_port_uart_init(void* uart_handle, kt_uint32_t baudrate,
                              kt_uart_data_t data_bits, kt_uart_stop_t stop_bits,
                              kt_uart_parity_t parity)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    
    /* 检查参数 */
    if (huart == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    /* UART已在CubeMX中配置，这里只需确保已初始化 */
    /* 如果需要动态配置，可以在这里修改huart->Init结构 */
    
    return KT_OK;
}

/**
 * @brief 通过UART发送数据（阻塞）
 */
kt_int32_t kt_port_uart_send(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    HAL_StatusTypeDef status;
    
    if (huart == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    status = HAL_UART_Transmit(huart, (uint8_t*)data, length, HAL_MAX_DELAY);
    
    return (status == HAL_OK) ? (kt_int32_t)length : 0;
}

/**
 * @brief 通过UART接收数据（阻塞）
 */
kt_int32_t kt_port_uart_receive(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    HAL_StatusTypeDef status;
    
    if (huart == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    status = HAL_UART_Receive(huart, data, length, HAL_MAX_DELAY);
    
    return (status == HAL_OK) ? (kt_int32_t)length : 0;
}

/**
 * @brief 通过UART发送数据（非阻塞）
 */
kt_status_t kt_port_uart_send_nb(void* uart_handle, const kt_uint8_t* data, kt_uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    HAL_StatusTypeDef status;
    
    if (huart == NULL || data == NULL || length == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    status = HAL_UART_Transmit_IT(huart, (uint8_t*)data, length);
    
    return (status == HAL_OK) ? KT_OK : KT_ERR_FAIL;
}

/**
 * @brief 通过UART接收数据（非阻塞）
 */
kt_int32_t kt_port_uart_receive_nb(void* uart_handle, kt_uint8_t* data, kt_uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    HAL_StatusTypeDef status;
    
    if (huart == NULL || data == NULL || length == 0)
    {
        return 0;
    }
    
    status = HAL_UART_Receive_IT(huart, data, length);
    
    return (status == HAL_OK) ? (kt_int32_t)length : 0;
}

#endif /* USE_KT_PLATFORM_STM32_HAL */