# ESP32 LED + Button 示例

本示例演示如何在ESP32平台上使用ktlib的LED和按键驱动。

## 硬件连接

### LED
- LED正极 → GPIO引脚（如GPIO2）
- LED负极 → GND（通过限流电阻）

### 按键
- 按键一端 → GPIO引脚（如GPIO4）
- 按键另一端 → GND
- 启用内部上拉电阻

## 使用方法

1. 将ktlib库添加到您的ESP32项目中
2. 选择ESP32平台：在 `kt_config.h` 中定义 `USE_KT_PLATFORM_ESP32`
3. 根据您的硬件修改GPIO引脚定义
4. 在主循环中定期调用 `kt_led_tick()` 和 `kt_button_scan()`

## 代码示例

```c
#include "kt_led.h"
#include "kt_button.h"
#include "kt_port.h"

/* LED实例 */
static kt_led_t led1;

/* 按键实例 */
static kt_button_t btn1;

/* 按键事件回调 */
void button_callback(void* arg, kt_button_event_t event)
{
    switch (event)
    {
        case KT_BUTTON_EVENT_SINGLE_CLICK:
            /* 单击：切换LED状态 */
            kt_led_toggle(&led1);
            break;
            
        case KT_BUTTON_EVENT_DOUBLE_CLICK:
            /* 双击：LED闪烁 */
            kt_led_blink(&led1, 200, 200);
            break;
            
        case KT_BUTTON_EVENT_LONG_PRESS:
            /* 长按：LED熄灭 */
            kt_led_off(&led1);
            break;
            
        default:
            break;
    }
}

void app_main(void)
{
    /* 初始化LED（使用GPIO2） */
    kt_led_init(&led1, 0, NULL, 2, NULL);
    
    /* 初始化按键（使用GPIO4，低电平有效） */
    kt_button_init(&btn1, 0, NULL, 4, KT_FALSE);
    kt_button_set_callback(&btn1, button_callback, NULL);
    
    while (1)
    {
        /* 处理LED效果 */
        kt_led_tick(&led1);
        
        /* 扫描按键 */
        kt_button_scan(&btn1);
        
        /* 延时10ms */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

## 注意事项

1. 确保在 `kt_config.h` 中启用了相关功能：
   - `KT_DRIVER_LED_ENABLE` = 1
   - `KT_DRIVER_BUTTON_ENABLE` = 1

2. ESP32的GPIO编号范围是0-39

3. 某些GPIO有特殊用途（如GPIO6-11用于SPI Flash），请避免使用

## 预期效果

- 单击按键：LED状态切换（开/关）
- 双击按键：LED开始闪烁
- 长按按键：LED关闭