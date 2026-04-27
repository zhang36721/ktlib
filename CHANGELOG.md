# Changelog

All notable changes to this project will be documented in this file.

## [0.1.0] - 2024-01-01

### Added
- 初始化 ktlib 工程结构
- 基础目录结构搭建完成
  - `kt_core/`: 核心基础模块（类型定义、配置、错误码、调试、工具）
  - `kt_port/`: 平台适配层（STM32 HAL、ESP32、Mock）
  - `kt_components/`: 通用组件（环形缓冲区、软件定时器、CRC校验）
  - `kt_drivers/`: 应用层驱动（LED、按键、蜂鸣器、UART协议）
  - `examples/`: 示例工程
- 修正 `docs/porting_guide.md` 与 `kt_port.h` 接口不一致问题
- 整理 `kt_core/kt_types.h` 跨平台兼容性
  - 移除 `typeof` 等 GCC 扩展语法
  - `KT_SWAP` 宏改为需要显式指定类型
- 整理 `kt_port/kt_port.h` 为纯接口声明
- 实现 `kt_ringbuf` 组件
  - 支持单字节和批量读写
  - 支持 peek 操作
  - 完善的参数检查和错误处理
- 增加 `examples/mock_demo/main.c` 测试程序
  - 完整的单元测试覆盖
  - 测试初始化、读写、回绕、peek、reset等功能
- 增加 CMake 构建支持
- 增加 `.gitignore` 文件
- 增加 `CHANGELOG.md`

### Changed
- 统一错误码定义
- 优化 API 接口设计

### Fixed
- 修正文档与代码接口不一致问题
- 修正跨平台编译兼容性问题

### Known Issues
- 软件定时器和CRC组件尚未完全实现
- LED/Button/Buzzer驱动需要依赖硬件，暂未在Mock平台测试
- STM32/ESP32平台适配层需要实际硬件验证

### Todo
- [ ] 完善 `kt_crc` 组件实现
- [ ] 完善 `kt_soft_timer` 组件实现
- [ ] 实现 LED/Button/Buzzer 驱动
- [ ] 验证 STM32 HAL 平台适配
- [ ] 验证 ESP32 平台适配
- [ ] 添加更多单元测试
- [ ] 完善文档和示例