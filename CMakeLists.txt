cmake_minimum_required(VERSION 3.10)
project(ktlib VERSION 0.1.0 LANGUAGES C)

# C标准设置
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# 调试选项
option(KT_DEBUG "Enable debug output" OFF)
if(KT_DEBUG)
    add_definitions(-DKT_DEBUG_ENABLE)
endif()

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/kt_core
    ${CMAKE_SOURCE_DIR}/kt_port
    ${CMAKE_SOURCE_DIR}/kt_port/mock
    ${CMAKE_SOURCE_DIR}/kt_components
)

# kt_core 库（头文件库，因为都是内联或宏）
# 实际上我们需要将配置相关的源文件编译进去
# 但当前kt_core都是头文件，所以只需包含目录

# kt_components 库
add_library(kt_components STATIC
    kt_components/kt_ringbuf.c
    # 后续可以添加其他组件
    # kt_components/kt_crc.c
    # kt_components/kt_soft_timer.c
)

target_include_directories(kt_components PUBLIC
    ${CMAKE_SOURCE_DIR}/kt_core
    ${CMAKE_SOURCE_DIR}/kt_port
    ${CMAKE_SOURCE_DIR}/kt_port/mock
    ${CMAKE_SOURCE_DIR}/kt_components
)

# Mock平台适配层
add_library(kt_port_mock STATIC
    kt_port/mock/kt_port_gpio_mock.c
    kt_port/mock/kt_port_uart_mock.c
    kt_port/mock/kt_port_time_mock.c
)

target_include_directories(kt_port_mock PUBLIC
    ${CMAKE_SOURCE_DIR}/kt_core
    ${CMAKE_SOURCE_DIR}/kt_port
    ${CMAKE_SOURCE_DIR}/kt_port/mock
    ${CMAKE_SOURCE_DIR}/kt_components
)

# 定义使用Mock平台
target_compile_definitions(kt_port_mock PUBLIC USE_KT_PLATFORM_MOCK)

# Mock Demo 可执行文件
add_executable(mock_demo examples/mock_demo/main.c)

target_link_libraries(mock_demo
    kt_components
    kt_port_mock
)

# 安装规则
install(TARGETS mock_demo DESTINATION bin)

# 打印配置信息
message(STATUS "ktlib Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Debug: ${KT_DEBUG}")
message(STATUS "  C Standard: ${CMAKE_C_STANDARD}")