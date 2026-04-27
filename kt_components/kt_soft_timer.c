/**
 * @file kt_soft_timer.c
 * @author kento
 * @brief 软件定时器实现
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024 kento
 */

#include "kt_soft_timer.h"
#include "kt_port.h"

#if KT_COMPONENT_SOFT_TIMER_ENABLE

static kt_bool_t timer_module_initialized = KT_FALSE;

/**
 * @brief 初始化软件定时器模块
 */
kt_status_t kt_soft_timer_init(void)
{
    timer_module_initialized = KT_TRUE;
    return KT_OK;
}

/**
 * @brief 反初始化软件定时器模块
 */
void kt_soft_timer_deinit(void)
{
    timer_module_initialized = KT_FALSE;
}

/**
 * @brief 创建定时器
 */
kt_status_t kt_soft_timer_create(kt_soft_timer_t* timer, kt_uint32_t id)
{
    if (timer == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    timer->id = id;
    timer->state = KT_TIMER_STATE_STOPPED;
    timer->mode = KT_TIMER_MODE_ONCE;
    timer->interval = 0;
    timer->start_time = 0;
    timer->callback = NULL;
    timer->arg = NULL;
    timer->active = KT_FALSE;
    
    return KT_OK;
}

/**
 * @brief 启动定时器
 */
kt_status_t kt_soft_timer_start(kt_soft_timer_t* timer, kt_uint32_t interval,
                                kt_timer_mode_t mode, kt_timer_callback_t callback,
                                void* arg)
{
    if (timer == NULL || interval == 0)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    timer->interval = interval;
    timer->mode = mode;
    timer->callback = callback;
    timer->arg = arg;
    timer->start_time = kt_port_get_tick();
    timer->state = KT_TIMER_STATE_RUNNING;
    timer->active = KT_TRUE;
    
    return KT_OK;
}

/**
 * @brief 停止定时器
 */
kt_status_t kt_soft_timer_stop(kt_soft_timer_t* timer)
{
    if (timer == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    timer->state = KT_TIMER_STATE_STOPPED;
    timer->active = KT_FALSE;
    
    return KT_OK;
}

/**
 * @brief 重置定时器
 */
kt_status_t kt_soft_timer_reset(kt_soft_timer_t* timer)
{
    if (timer == NULL)
    {
        return KT_ERR_INVALID_PARAM;
    }
    
    timer->state = KT_TIMER_STATE_STOPPED;
    timer->start_time = 0;
    timer->active = KT_FALSE;
    
    return KT_OK;
}

/**
 * @brief 检查定时器是否超时
 */
kt_bool_t kt_soft_timer_is_expired(kt_soft_timer_t* timer)
{
    kt_tick_t current_time;
    kt_tick_t elapsed;
    
    if (timer == NULL || !timer->active)
    {
        return KT_FALSE;
    }
    
    if (timer->state != KT_TIMER_STATE_RUNNING)
    {
        return KT_FALSE;
    }
    
    current_time = kt_port_get_tick();
    elapsed = current_time - timer->start_time;
    
    if (elapsed >= timer->interval)
    {
        return KT_TRUE;
    }
    
    return KT_FALSE;
}

/**
 * @brief 获取定时器剩余时间
 */
kt_uint32_t kt_soft_timer_get_remaining(kt_soft_timer_t* timer)
{
    kt_tick_t current_time;
    kt_tick_t elapsed;
    
    if (timer == NULL || !timer->active)
    {
        return 0;
    }
    
    if (timer->state != KT_TIMER_STATE_RUNNING)
    {
        return 0;
    }
    
    current_time = kt_port_get_tick();
    elapsed = current_time - timer->start_time;
    
    if (elapsed >= timer->interval)
    {
        return 0;
    }
    
    return (kt_uint32_t)(timer->interval - elapsed);
}

/**
 * @brief 定时器心跳处理
 */
void kt_soft_timer_tick(void)
{
    /* 这里可以遍历所有定时器并处理超时 */
    /* 实际使用中可以在主循环中调用此函数 */
}

/**
 * @brief 获取定时器状态
 */
kt_timer_state_t kt_soft_timer_get_state(kt_soft_timer_t* timer)
{
    if (timer == NULL)
    {
        return KT_TIMER_STATE_STOPPED;
    }
    
    return timer->state;
}

/**
 * @brief 判断定时器是否正在运行
 */
kt_bool_t kt_soft_timer_is_running(kt_soft_timer_t* timer)
{
    if (timer == NULL)
    {
        return KT_FALSE;
    }
    
    return (timer->active && timer->state == KT_TIMER_STATE_RUNNING) ? KT_TRUE : KT_FALSE;
}

#endif /* KT_COMPONENT_SOFT_TIMER_ENABLE */