/**
 * @file    led_flow.c
 * @brief   流水灯非阻塞状态机实现（题目 5）。
 *          效果播放路径上不使用 HAL_Delay()，全部通过单次触发定时器调度。
 */
#include "led_flow.h"
#include "led.h"            /* 仅在实现文件中包含底层 LED 驱动，main.c 不 include */

/* 模块内部状态变量 */
static soft_timer_t s_step_timer;                            /* 步骤切换定时器 */
static uint8_t      s_step_index  = 0U;                      /* 当前步骤索引 */
static led_flow_mode_t s_current_mode = LED_FLOW_MODE_IDLE;  /* 当前实际运行模式 */

/**
 * @brief  启动单次触发软件定时器
 * @param  timer:     定时器结构体
 * @param  period_ms: 定时时长，单位 ms
 * @note   题目 5 要求 2：基于 HAL_GetTick() 封装计时器。
 */
void timer_start(soft_timer_t *timer, uint32_t period_ms)
{
    timer->start_ms = HAL_GetTick();
    timer->period_ms = period_ms;
    timer->running = 1U;
}

/**
 * @brief  查询单次触发定时器是否到期
 * @param  timer: 定时器结构体
 * @retval 1 表示到期，0 表示未到期
 * @note   题目 5 要求 3：计时器是单次触发。
 *         到期后自动把 running 清 0，不重新启动就不会再次触发。
 */
uint8_t timer_is_expired(soft_timer_t *timer)
{
    if (!timer->running)
    {
        return 0U;
    }

    if ((HAL_GetTick() - timer->start_ms) >= timer->period_ms)
    {
        timer->running = 0U;    /* 单次触发：到期自动停止 */
        return 1U;
    }

    return 0U;
}

/**
 * @brief  熄灭所有 LED
 * @note   内部辅助函数，循环调用 led_off() 实现
 */
static void all_leds_off(void)
{
    for (uint8_t i = 1U; i <= LED_FLOW_LED_COUNT; i++)
    {
        led_off(i);
    }
}

/**
 * @brief  点亮所有 LED
 * @note   内部辅助函数，循环调用 led_on() 实现
 */
static void all_leds_on(void)
{
    for (uint8_t i = 1U; i <= LED_FLOW_LED_COUNT; i++)
    {
        led_on(i);
    }
}

/**
 * @brief  模式切换或进入新状态时复位内部状态
 * @note   题目 5 要求 5：signal 变化后下一轮立刻进入新状态。
 *         这里关闭所有 LED、重置步骤索引，并启动一个 0ms 定时器让下一轮立即执行新状态第一步。
 */
static void led_flow_reset(void)
{
    all_leds_off();
    s_step_index = 0U;
    timer_start(&s_step_timer, 0U);  /* 立即到期，下一轮进入新状态第一步 */
}

/**
 * @brief  模式 1：逐个亮灭（非阻塞）
 * @note   题目 5 要求 4：每轮只执行一步，用定时器决定何时进入下一步。
 */
static void update_mode_single(void)
{
    switch (s_step_index)
    {
        case 0U:  led_on(1);  break;
        case 1U:  led_off(1); break;
        case 2U:  led_on(2);  break;
        case 3U:  led_off(2); break;
        case 4U:  led_on(3);  break;
        case 5U:  led_off(3); break;
        case 6U:  led_on(4);  break;
        case 7U:  led_off(4); break;
        default:  break;
    }

    s_step_index++;
    if (s_step_index > 7U)
    {
        s_step_index = 0U;
    }

    timer_start(&s_step_timer, 250U);  /* 250ms 后执行下一步 */
}

/**
 * @brief  模式 2：两两亮灭（非阻塞）
 */
static void update_mode_pair(void)
{
    switch (s_step_index)
    {
        case 0U:
            led_on(1);
            led_on(2);
            break;
        case 1U:
            led_off(1);
            led_off(2);
            break;
        case 2U:
            led_on(3);
            led_on(4);
            break;
        case 3U:
            led_off(3);
            led_off(4);
            break;
        default:
            break;
    }

    s_step_index++;
    if (s_step_index > 3U)
    {
        s_step_index = 0U;
    }

    timer_start(&s_step_timer, 250U);
}

/**
 * @brief  模式 3：一起亮灭（非阻塞）
 */
static void update_mode_all(void)
{
    switch (s_step_index)
    {
        case 0U:
            all_leds_on();
            break;
        case 1U:
            all_leds_off();
            break;
        default:
            break;
    }

    s_step_index++;
    if (s_step_index > 1U)
    {
        s_step_index = 0U;
    }

    timer_start(&s_step_timer, 250U);
}

/**
 * @brief  非阻塞流水灯状态机更新函数
 * @note   题目 5 要求 4：main 的 while(1) 每轮调用一次本函数。
 *         题目 5 要求 5：signal 变化后立刻切换，不等当前步骤播完。
 */
void led_flow_update(void)
{
    /* 信号变化，立刻切换：不需要等当前步骤播完 */
    if (signal != s_current_mode)
    {
        s_current_mode = signal;
        led_flow_reset();
    }

    /* 时间未到，保持当前输出不变，直接返回 */
    if (!timer_is_expired(&s_step_timer))
    {
        return;
    }

    /* 时间到了，根据当前模式执行下一步 */
    switch (s_current_mode)
    {
        case LED_FLOW_MODE_IDLE:
            all_leds_off();     /* 0：四颗 LED 全灭 */
            /* IDLE 不需要启动定时器，每轮进来都保持全灭 */
            break;

        case LED_FLOW_MODE_SINGLE:
            update_mode_single();
            break;

        case LED_FLOW_MODE_PAIR:
            update_mode_pair();
            break;

        case LED_FLOW_MODE_ALL:
            update_mode_all();
            break;

        default:
            led_flow_reset();
            break;
    }
}
