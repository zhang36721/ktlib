# ktlib 移植指南

本文档详细说明如何将 ktlib 移植到不同的 MCU 平台。

## 1. 移植概述

ktlib 通过 `kt_port` 适配层实现跨平台兼容。移植的核心工作是为目标平台实现 `kt_port` 接口。

### 1.1 需要实现的接口

| 接口类别 | 必需函数 | 说明 |
|----------|----------|------|
| GPIO | `kt_port_gpio_init()` | GPIO初始化 |
| | `kt_port_gpio_write()` | GPIO写操作 |
| | `kt_port_gpio_read()` | GPIO读操作 |
| | `kt_port_gpio_toggle()` | GPIO切换操作 |
| UART | `kt_port_uart_init()` | UART初始化 |
| | `kt_port_uart_send()` | UART发送数据（阻塞） |
| | `kt_port_uart_receive()` | UART接收数据（阻塞） |
| | `kt_port_uart_send_nb()` | UART发送数据（非阻塞） |
| | `kt_port_uart_receive_nb()` | UART接收数据（非阻塞） |
| Time | `kt_port_delay_ms()` | 毫秒级延时 |
| | `kt_port_delay_us()` | 微秒级延时 |
| | `kt_port_get_tick()` | 获取系统时钟节拍（毫秒） |
| | `kt_port_get_tick_freq()` | 获取系统时钟频率 |

### 1.2 UART 接口详细说明

```c
/**
 * @brief 初始化UART
 * @param uart_handle UART句柄（平台相关）
 * @param baudrate 波特率，如 115200
 * @param data_bits 数据位，取值：
 *   - KT_UART_DATA_5 (5位)
 *   - KT_UART_DATA_6 (6位)
 *   - KT_UART_DATA_7 (7位)
 *   - KT_UART_DATA_8 (8位，最常用)
 * @param stop_bits 停止位，取值：
 *   - KT_UART_STOP_1 (1位停止位，最常用)
 *   - KT_UART_STOP_2 (2位停止位)
 * @param parity 校验位，取值：
 *   - KT_UART_PARITY_NONE (无校验，最常用)
 *   - KT_UART_PARITY_EVEN (偶校验)
 *   - KT_UART_PARITY_ODD (奇校验)
 * @return KT_OK 成功，其他 失败
 */
kt_status_t kt_port_uart_init(void* uart_handle, kt_uint32_t baudrate,
                              kt_uart_data_t data_bits, 
                              kt_uart_stop_t stop_bits,
                              kt_uart_parity_t parity);
```

## 2. STM32 HAL 平台移植

### 2.1 目录结构

```
kt_port/stm32_hal/
├── kt_port_gpio_stm32.c
├── kt_port_uart_stm32.c
└── kt_port_time_stm32.c
```

### 2.2 GPIO 适配实现

```c
// kt_port/stm32_hal/kt_port_gpio_stm32.c

#include "kt_port.h"
#include "stm32f4xx_hal.h"  // 根据具体型号调整

/**
 * @brief 初始化GPIO引脚
 * @param port GPIO端口（如GPIOA, GPIOB等）
 * @param pin GPIO引脚号（0-15）
 * @param mode 模式（输入/输出）
 * @param pull 上下拉配置
 * @return KT_OK 成功，KT_ERROR 失败
 */
kt_status_t kt_port_gpio_init(void* port, uint16_t pin, 
                               kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    
    // 使能GPIO时钟（需要根据端口号使能对应时钟）
    // __HAL_RCC_GPIOA_CLK_ENABLE();  // 示例
    
    GPIO_InitStruct.Pin = (1U << pin);
    GPIO_InitStruct.Mode = (mode == KT_GPIO_MODE_OUTPUT) ? 
                           GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = (pull == KT_GPIO_PULL_UP) ? GPIO_PULLUP :
                           (pull == KT_GPIO_PULL_DOWN) ? GPIO_PULLDOWN : GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
    
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚
 * @param port GPIO端口
 * @param pin GPIO引脚号
 * @param value 写入值（0或1）
 */
void kt_port_gpio_write(void* port, uint16_t pin, uint8_t value)
{
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    HAL_GPIO_WritePin(gpio_port, (1U << pin), 
                      value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief 读取GPIO引脚
 * @param port GPIO端口
 * @param pin GPIO引脚号
 * @return 读取值（0或1）
 */
uint8_t kt_port_gpio_read(void* port, uint16_t pin)
{
    GPIO_TypeDef* gpio_port = (GPIO_TypeDef*)port;
    return HAL_GPIO_ReadPin(gpio_port, (1U << pin)) == GPIO_PIN_SET ? 1 : 0;
}
```

### 2.3 UART 适配实现

```c
// kt_port/stm32_hal/kt_port_uart_stm32.c

#include "kt_port.h"
#include "stm32f4xx_hal.h"

/**
 * @brief 初始化UART
 * @param uart_handle UART句柄指针
 * @param baudrate 波特率
 * @return KT_OK 成功，KT_ERROR 失败
 */
kt_status_t kt_port_uart_init(void* uart_handle, uint32_t baudrate)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    // UART初始化已在CubeMX中配置，这里只需确保已初始化
    return KT_OK;
}

/**
 * @brief 通过UART发送数据
 * @param uart_handle UART句柄指针
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 实际发送的字节数
 */
int kt_port_uart_send(void* uart_handle, const uint8_t* data, uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    if (HAL_UART_Transmit(huart, (uint8_t*)data, length, HAL_MAX_DELAY) == HAL_OK)
    {
        return length;
    }
    return 0;
}

/**
 * @brief 通过UART接收数据
 * @param uart_handle UART句柄指针
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return 实际接收的字节数
 */
int kt_port_uart_receive(void* uart_handle, uint8_t* data, uint16_t length)
{
    UART_HandleTypeDef* huart = (UART_HandleTypeDef*)uart_handle;
    if (HAL_UART_Receive(huart, data, length, HAL_MAX_DELAY) == HAL_OK)
    {
        return length;
    }
    return 0;
}
```

### 2.4 时间适配实现

```c
// kt_port/stm32_hal/kt_port_time_stm32.c

#include "kt_port.h"
#include "stm32f4xx_hal.h"

/**
 * @brief 毫秒级延时
 * @param ms 延时毫秒数
 */
void kt_port_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief 获取系统时钟节拍（毫秒）
 * @return 系统时钟节拍值
 */
uint32_t kt_port_get_tick(void)
{
    return HAL_GetTick();
}
```

## 3. ESP32 平台移植

### 3.1 目录结构

```
kt_port/esp32/
├── kt_port_gpio_esp32.c
├── kt_port_uart_esp32.c
└── kt_port_time_esp32.c
```

### 3.2 GPIO 适配实现

```c
// kt_port/esp32/kt_port_gpio_esp32.c

#include "kt_port.h"
#include "driver/gpio.h"

/**
 * @brief 初始化GPIO引脚
 */
kt_status_t kt_port_gpio_init(void* port, uint16_t pin, 
                               kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    gpio_config_t io_conf = {0};
    io_conf.pin_bit_mask = (1ULL << pin);
    
    if (mode == KT_GPIO_MODE_OUTPUT)
    {
        io_conf.mode = GPIO_MODE_OUTPUT;
    }
    else
    {
        io_conf.mode = GPIO_MODE_INPUT;
    }
    
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
        default:
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
    }
    
    gpio_config(&io_conf);
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚
 */
void kt_port_gpio_write(void* port, uint16_t pin, uint8_t value)
{
    gpio_set_level(pin, value);
}

/**
 * @brief 读取GPIO引脚
 */
uint8_t kt_port_gpio_read(void* port, uint16_t pin)
{
    return gpio_get_level(pin);
}
```

### 3.3 UART 适配实现

```c
// kt_port/esp32/kt_port_uart_esp32.c

#include "kt_port.h"
#include "driver/uart.h"

/**
 * @brief 初始化UART
 */
kt_status_t kt_port_uart_init(void* uart_handle, uint32_t baudrate)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    uart_param_config(uart_num, &uart_config);
    uart_driver_install(uart_num, 256, 0, 0, NULL, 0);
    
    return KT_OK;
}

/**
 * @brief 通过UART发送数据
 */
int kt_port_uart_send(void* uart_handle, const uint8_t* data, uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    return uart_write_bytes(uart_num, data, length);
}

/**
 * @brief 通过UART接收数据
 */
int kt_port_uart_receive(void* uart_handle, uint8_t* data, uint16_t length)
{
    uart_port_t uart_num = (uart_port_t)uart_handle;
    return uart_read_bytes(uart_num, data, length, portMAX_DELAY);
}
```

### 3.4 时间适配实现

```c
// kt_port/esp32/kt_port_time_esp32.c

#include "kt_port.h"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"

/**
 * @brief 毫秒级延时
 */
void kt_port_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief 获取系统时钟节拍（毫秒）
 */
uint32_t kt_port_get_tick(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}
```

## 4. 8051（51单片机）平台移植

### 4.1 目录结构

```
kt_port/8051/
├── kt_port_gpio_8051.c
├── kt_port_uart_8051.c
└── kt_port_time_8051.c
```

### 4.2 注意事项

8051 平台资源有限，移植时需注意：

1. **数据类型**：8051 的 int 通常是 16 位，需要确保 `kt_types.h` 中的类型定义正确
2. **内存**：RAM 非常有限，避免使用大的缓冲区和复杂的结构体
3. **时钟**：通常没有操作系统，需要自己实现延时函数

### 4.3 GPIO 适配实现（示例）

```c
// kt_port/8051/kt_port_gpio_8051.c

#include "kt_port.h"

// 根据实际8051型号定义GPIO寄存器
// 这里以STC89C52为例

/**
 * @brief 初始化GPIO引脚
 */
kt_status_t kt_port_gpio_init(void* port, uint16_t pin, 
                               kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    // 8051的GPIO通常是准双向口，不需要特别配置
    // 这里可以根据需要配置弱上拉等
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚
 */
void kt_port_gpio_write(void* port, uint16_t pin, uint8_t value)
{
    // 根据port和pin写入对应的IO口
    // 例如：P1 = (P1 & ~(1 << pin)) | (value << pin);
}

/**
 * @brief 读取GPIO引脚
 */
uint8_t kt_port_gpio_read(void* port, uint16_t pin)
{
    // 根据port和pin读取对应的IO口
    // 例如：return (P1 >> pin) & 1;
    return 0;
}
```

## 5. Mock（模拟测试）平台

### 5.1 用途

Mock 平台用于在 PC 上模拟嵌入式环境，便于：
- 在没有硬件的情况下开发和测试代码
- 单元测试
- 算法验证

### 5.2 目录结构

```
kt_port/mock/
├── kt_port_gpio_mock.c
├── kt_port_uart_mock.c
└── kt_port_time_mock.c
```

### 5.3 GPIO 模拟实现

```c
// kt_port/mock/kt_port_gpio_mock.c

#include "kt_port.h"
#include <stdio.h>
#include <stdlib.h>

// 模拟GPIO状态
static uint8_t mock_gpio_state[32] = {0};

/**
 * @brief 初始化GPIO引脚（模拟）
 */
kt_status_t kt_port_gpio_init(void* port, uint16_t pin, 
                               kt_gpio_mode_t mode, kt_gpio_pull_t pull)
{
    printf("[MOCK] GPIO init: port=%p, pin=%d, mode=%d, pull=%d\n", 
           port, pin, mode, pull);
    return KT_OK;
}

/**
 * @brief 写入GPIO引脚（模拟）
 */
void kt_port_gpio_write(void* port, uint16_t pin, uint8_t value)
{
    printf("[MOCK] GPIO write: port=%p, pin=%d, value=%d\n", port, pin, value);
    if (pin < 32)
    {
        mock_gpio_state[pin] = value;
    }
}

/**
 * @brief 读取GPIO引脚（模拟）
 */
uint8_t kt_port_gpio_read(void* port, uint16_t pin)
{
    printf("[MOCK] GPIO read: port=%p, pin=%d\n", port, pin);
    if (pin < 32)
    {
        return mock_gpio_state[pin];
    }
    return 0;
}
```

### 5.4 UART 模拟实现

```c
// kt_port/mock/kt_port_uart_mock.c

#include "kt_port.h"
#include <stdio.h>

/**
 * @brief 初始化UART（模拟）
 */
kt_status_t kt_port_uart_init(void* uart_handle, uint32_t baudrate)
{
    printf("[MOCK] UART init: handle=%p, baudrate=%lu\n", uart_handle, baudrate);
    return KT_OK;
}

/**
 * @brief 通过UART发送数据（模拟）
 */
int kt_port_uart_send(void* uart_handle, const uint8_t* data, uint16_t length)
{
    printf("[MOCK] UART send: ");
    for (uint16_t i = 0; i < length; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return length;
}

/**
 * @brief 通过UART接收数据（模拟）
 */
int kt_port_uart_receive(void* uart_handle, uint8_t* data, uint16_t length)
{
    printf("[MOCK] UART receive: waiting for data...\n");
    // 模拟接收，实际使用时可以读取stdin
    return 0;
}
```

### 5.5 时间模拟实现

```c
// kt_port/mock/kt_port_time_mock.c

#include "kt_port.h"
#include <time.h>
#include <stdio.h>

/**
 * @brief 毫秒级延时（模拟）
 */
void kt_port_delay_ms(uint32_t ms)
{
    printf("[MOCK] Delay %u ms\n", ms);
    // 在PC上可以使用系统延时
    // 注意：这会影响性能，仅用于测试
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}

/**
 * @brief 获取系统时钟节拍（模拟）
 */
uint32_t kt_port_get_tick(void)
{
    #ifdef _WIN32
        return GetTickCount();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    #endif
}
```

## 6. 自定义平台移植步骤

如果你需要将 ktlib 移植到其他平台，请按以下步骤操作：

### 步骤 1：创建平台目录

在 `kt_port/` 下创建你的平台目录，例如 `kt_port/my_mcu/`

### 步骤 2：复制模板文件

从现有平台（如 mock）复制文件作为模板：
- `kt_port_gpio_xxx.c`
- `kt_port_uart_xxx.c`
- `kt_port_time_xxx.c`

### 步骤 3：实现接口函数

根据你的 MCU 的硬件特性和 SDK，实现所有接口函数。

### 步骤 4：修改 kt_port.h

在 `kt_port.h` 中添加条件编译，选择你的平台实现：

```c
// kt_port.h

#if defined(USE_STM32_HAL)
    #include "stm32_hal/kt_port_gpio_stm32.c"
    #include "stm32_hal/kt_port_uart_stm32.c"
    #include "stm32_hal/kt_port_time_stm32.c"
#elif defined(USE_ESP32)
    #include "esp32/kt_port_gpio_esp32.c"
    #include "esp32/kt_port_uart_esp32.c"
    #include "esp32/kt_port_time_esp32.c"
#elif defined(USE_MY_MCU)  // 添加你的平台
    #include "my_mcu/kt_port_gpio_my_mcu.c"
    #include "my_mcu/kt_port_uart_my_mcu.c"
    #include "my_mcu/kt_port_time_my_mcu.c"
#else
    #include "mock/kt_port_gpio_mock.c"
    #include "mock/kt_port_uart_mock.c"
    #include "mock/kt_port_time_mock.c"
#endif
```

### 步骤 5：测试验证

编写简单的测试代码验证移植是否正确：

```c
#include "kt_port.h"

int main(void)
{
    // 测试GPIO
    kt_port_gpio_init(GPIOA, 5, KT_GPIO_MODE_OUTPUT, KT_GPIO_PULL_NONE);
    kt_port_gpio_write(GPIOA, 5, 1);
    
    // 测试延时
    kt_port_delay_ms(1000);
    
    // 测试UART
    const char* msg = "Hello ktlib!\r\n";
    kt_port_uart_send(UART1, (uint8_t*)msg, strlen(msg));
    
    return 0;
}
```

## 7. 常见问题

### Q1: 如何处理不同平台的类型差异？

A: ktlib 在 `kt_types.h` 中定义了统一的类型，如 `kt_int8_t`、`kt_uint32_t` 等。这些类型会根据平台自动映射到合适的底层类型。

### Q2: 如果我的平台没有操作系统怎么办？

A: 对于裸机系统，`kt_port_get_tick()` 可以基于硬件定时器实现。`kt_port_delay_ms()` 可以使用简单的循环延时或定时器中断实现。

### Q3: 如何调试移植问题？

A: 建议先在 Mock 平台上开发和测试上层代码，确保逻辑正确后再移植到目标平台。移植过程中可以使用串口打印调试信息。

### Q4: 可以只实现部分接口吗？

A: 可以。如果你不使用 UART 功能，可以不实现 UART 相关接口。但未实现的接口对应的功能将无法使用。

## 8. 总结

ktlib 的移植工作主要集中在 `kt_port` 层。通过实现 GPIO、UART 和 Time 三大类接口，即可让 ktlib 在你的平台上运行。

如果在移植过程中遇到问题，欢迎通过 Issue 反馈！