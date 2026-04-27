# ktlib - 通用嵌入式应用层驱动库

## 项目简介

**ktlib** 是一套面向嵌入式应用层开发的轻量级通用驱动库。

### 项目定位

- **跨平台兼容**：通过 `kt_port` 适配层隔离不同MCU平台差异
- **轻量级设计**：不依赖RTOS，资源占用少
- **高可移植性**：同一套上层驱动代码可在STM32、ESP32、51单片机等平台上复用
- **模块化架构**：清晰的层次划分，便于扩展和维护

### 支持的platform

- STM32 (HAL库)
- ESP32
- 8051 (51单片机)
- Mock (模拟测试环境)

## 目录结构

```
ktlib/
├── README.md              # 本文件
├── LICENSE                # MIT许可证
├── docs/                  # 文档目录
│   ├── ktlib_design.md    # 架构设计文档
│   └── porting_guide.md   # 移植指南
├── kt_core/               # 核心基础模块
│   ├── kt_types.h         # 基础类型定义
│   ├── kt_config.h        # 配置宏定义
│   ├── kt_error.h         # 错误码定义
│   ├── kt_debug.h         # 调试工具
│   └── kt_utils.h         # 工具函数
├── kt_port/               # 平台适配层
│   ├── kt_port.h          # 适配层接口声明
│   ├── stm32_hal/         # STM32 HAL库适配
│   ├── esp32/             # ESP32适配
│   └── mock/              # 模拟测试适配
├── kt_components/         # 通用组件
│   ├── kt_ringbuf.h/.c    # 环形缓冲区
│   ├── kt_soft_timer.h/.c # 软件定时器
│   └── kt_crc.h/.c        # CRC校验
├── kt_drivers/            # 应用层驱动
│   ├── led/               # LED驱动
│   ├── button/            # 按键驱动
│   ├── buzzer/            # 蜂鸣器驱动
│   └── uart_protocol/     # UART通信协议
└── examples/              # 示例工程
    ├── stm32_led_button_demo/
    ├── esp32_led_button_demo/
    └── mock_demo/
```

## 模块说明

### kt_core - 核心基础模块
提供整个库的基础支撑：
- `kt_types.h`: 统一的数据类型定义
- `kt_config.h`: 可配置的宏定义
- `kt_error.h`: 统一的错误码系统
- `kt_debug.h`: 调试打印和断言
- `kt_utils.h`: 常用工具函数

### kt_port - 平台适配层
隔离不同MCU平台的硬件差异：
- GPIO操作接口
- UART操作接口
- 时间/延时接口

### kt_components - 通用组件
可复用的软件组件：
- 环形缓冲区（用于数据缓存）
- 软件定时器（提供定时功能）
- CRC校验（数据完整性校验）

### kt_drivers - 应用层驱动
面向应用的驱动程序：
- LED驱动（支持闪烁、呼吸等效果）
- 按键驱动（支持单击、双击、长按等）
- 蜂鸣器驱动（支持不同音调）
- UART协议（简单的通信协议框架）

## 第一阶段目标

✅ 完成基础目录结构搭建  
✅ 创建所有头文件和源文件的框架  
✅ 编写项目文档和移植指南  
✅ 建立清晰的代码组织结构  

## 后续计划

### Phase 2 - 核心组件实现
- [x] 实现 `kt_ringbuf` 环形缓冲区 ✅
- [ ] 实现 `kt_crc` CRC校验
- [ ] 实现 `kt_soft_timer` 软件定时器

### Phase 3 - 基础驱动实现
- [ ] 实现 `kt_led` LED驱动
- [ ] 实现 `kt_button` 按键驱动
- [ ] 实现 `kt_buzzer` 蜂鸣器驱动

### Phase 4 - 平台适配
- [ ] 完成STM32 HAL库平台适配
- [ ] 完成ESP32平台适配
- [ ] 完善Mock测试环境

### Phase 5 - 高级功能
- [ ] 实现 `kt_uart_protocol` UART通信协议
- [ ] 添加更多传感器驱动
- [ ] 完善示例工程

## 快速开始

1. 将 `ktlib` 目录复制到你的项目中
2. 根据你的MCU平台，选择对应的 `kt_port` 适配层
3. 在 `kt_config.h` 中配置所需的功能模块
4. 包含相应的头文件并开始使用

## 移植指南

详细移植步骤请参考 `docs/porting_guide.md`

## 许可证

本项目采用 MIT 许可证，详见 `LICENSE` 文件。

## 贡献

欢迎提交Issue和Pull Request来帮助改进这个项目！

## 联系方式

作者：kento  
如有问题或建议，请通过Issue联系。