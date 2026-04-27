# Mock 模拟测试示例

本示例演示如何在PC上使用Mock平台测试ktlib的功能。

## 用途

Mock平台用于在没有实际硬件的情况下开发和测试ktlib代码，适用于：

- 算法验证
- 单元测试
- 代码调试
- CI/CD集成测试

## 使用方法

1. 确保使用Mock平台：在 `kt_config.h` 中不定义任何平台宏（默认使用Mock）
2. 在PC上编译运行测试代码
3. 观察控制台输出

## 代码示例

```c
#include <stdio.h>
#include "kt_led.h"
#include "kt_button.h"
#include "kt_ringbuf.h"
#include "kt_port.h"

/* LED实例 */
static kt_led_t led1;

/* 按键实例 */
static kt_button_t btn1;

/* 环形缓冲区 */
static kt_ringbuf_t ringbuf;
static uint8_t ringbuf_data[256];

/* 按键事件回调 */
void button_callback(void* arg, kt_button_event_t event)
{
    printf("Button event: ");
    switch (event)
    {
        case KT_BUTTON_EVENT_SINGLE_CLICK:
            printf("Single Click\n");
            kt_led_toggle(&led1);
            break;
            
        case KT_BUTTON_EVENT_DOUBLE_CLICK:
            printf("Double Click\n");
            kt_led_blink(&led1, 200, 200);
            break;
            
        case KT_BUTTON_EVENT_LONG_PRESS:
            printf("Long Press\n");
            kt_led_off(&led1);
            break;
            
        default:
            printf("Other\n");
            break;
    }
}

int main(void)
{
    printf("=== ktlib Mock Demo ===\n\n");
    
    /* 初始化LED */
    printf("Initializing LED...\n");
    kt_led_init(&led1, 0, NULL, 5, NULL);
    
    /* 初始化按键 */
    printf("Initializing Button...\n");
    kt_button_init(&btn1, 0, NULL, 0, KT_FALSE);
    kt_button_set_callback(&btn1, button_callback, NULL);
    
    /* 初始化环形缓冲区 */
    printf("Initializing Ring Buffer...\n");
    kt_ringbuf_init(&ringbuf, ringbuf_data, sizeof(ringbuf_data));
    
    /* 测试环形缓冲区 */
    printf("\nTesting Ring Buffer...\n");
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    kt_ringbuf_write(&ringbuf, test_data, sizeof(test_data));
    printf("Wrote %zu bytes to ring buffer\n", sizeof(test_data));
    printf("Data in buffer: %u\n", kt_ringbuf_data_len(&ringbuf));
    
    uint8_t read_data[5] = {0};
    kt_ringbuf_read(&ringbuf, read_data, sizeof(read_data));
    printf("Read data: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%02X ", read_data[i]);
    }
    printf("\n");
    
    /* 主循环 */
    printf("\nEntering main loop (press Ctrl+C to exit)...\n");
    int count = 0;
    while (count < 100)  /* 模拟运行100次循环 */
    {
        /* 处理LED效果 */
        kt_led_tick(&led1);
        
        /* 扫描按键 */
        kt_button_scan(&btn1);
        
        /* 模拟延时 */
        kt_port_delay_ms(10);
        
        count++;
    }
    
    printf("\nDemo finished!\n");
    
    return 0;
}
```

## 编译运行

### Linux/Mac

```bash
gcc -I../.. -o mock_demo main.c \
    ../../kt_core/*.c \
    ../../kt_port/mock/*.c \
    ../../kt_components/*.c \
    ../../kt_drivers/led/*.c \
    ../../kt_drivers/button/*.c \
    -lpthread

./mock_demo
```

### Windows (MinGW)

```bash
gcc -I../.. -o mock_demo.exe main.c ^
    ../../kt_core/*.c ^
    ../../kt_port/mock/*.c ^
    ../../kt_components/*.c ^
    ../../kt_drivers/led/*.c ^
    ../../kt_drivers/button/*.c

mock_demo.exe
```

## 预期输出

```
=== ktlib Mock Demo ===

Initializing LED...
[MOCK] GPIO init: port=(nil), pin=5, mode=1, pull=0
Initializing Button...
[MOCK] GPIO init: port=(nil), pin=0, mode=0, pull=1
Initializing Ring Buffer...

Testing Ring Buffer...
Wrote 5 bytes to ring buffer
Data in buffer: 5
Read data: 01 02 03 04 05 

Entering main loop (press Ctrl+C to exit)...

Demo finished!
```

## 注意事项

1. Mock平台会打印所有GPIO和UART操作的日志
2. Mock平台的时间函数使用系统时间
3. Mock平台适合功能验证，但不能完全替代真实硬件测试