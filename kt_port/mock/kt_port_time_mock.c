/**
 * @file kt_port_time_mock.c
 * @author kento
 * @brief Mock平台 时间模拟实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_port.h"

#ifdef USE_KT_PLATFORM_MOCK

#include <time.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

static kt_uint32_t mock_start_tick = 0;
static kt_bool_t mock_time_initialized = KT_FALSE;

/**
 * @brief 初始化Mock时间系统
 */
static void mock_time_init(void)
{
    if (!mock_time_initialized)
    {
        mock_start_tick = 0;
        mock_time_initialized = KT_TRUE;
    }
}

/**
 * @brief 毫秒级延时（模拟）
 */
void kt_port_delay_ms(kt_uint32_t ms)
{
    mock_time_init();
    printf("[MOCK] Delay %u ms\n", ms);
    
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

/**
 * @brief 微秒级延时（模拟）
 */
void kt_port_delay_us(kt_uint32_t us)
{
    mock_time_init();
    printf("[MOCK] Delay %u us\n", us);
    
#ifdef _WIN32
    /* Windows没有usleep，使用高精度等待 */
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    while (1)
    {
        QueryPerformanceCounter(&end);
        if ((end.QuadPart - start.QuadPart) * 1000000 / frequency.QuadPart >= us)
            break;
    }
#else
    usleep(us);
#endif
}

/**
 * @brief 获取系统时钟节拍（模拟）
 */
kt_tick_t kt_port_get_tick(void)
{
    mock_time_init();
    
#ifdef _WIN32
    return (kt_tick_t)GetTickCount();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (kt_tick_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

/**
 * @brief 获取系统时钟频率（模拟）
 */
kt_uint32_t kt_port_get_tick_freq(void)
{
    return 1000U;  /* 1kHz */
}

#endif /* USE_KT_PLATFORM_MOCK */