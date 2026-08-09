/**
 * @file    led_flow.h
 * @brief   流水灯模块公共接口（题目 5 非阻塞版本）。
 *          本头文件只暴露枚举、结构体和公共更新函数，
 *          不暴露 led_on()/led_off() 等底层 LED 驱动接口。
 */
#ifndef LED_FLOW_H
#define LED_FLOW_H

#include "stm32h7xx_hal.h"

#define LED_FLOW_LED_COUNT 4U   /* 流水灯使用的 LED 数量 */

/**
 * @brief 蜂鸣器一次鸣叫配置（题目 6）
 */
typedef struct
{
    uint16_t on_ms;     /* 响多久 */
    uint16_t off_ms;    /* 停多久 */
} buzzer_beat_t;

/**
 * @brief 一次亮灭的参数结构体（题目 3 保留）
 */
typedef struct
{
    uint8_t  led_num;   /* LED 编号，范围 1~LED_FLOW_LED_COUNT */
    uint16_t on_ms;     /* 点亮持续时间，单位 ms */
    uint16_t off_ms;    /* 熄灭持续时间，单位 ms */
} led_step_config_t;

/**
 * @brief 流水灯模式枚举
 * @note  题目 5：0 为 IDLE，1/2/3 对应三种流水灯模式
 */
typedef enum
{
    LED_FLOW_MODE_IDLE   = 0,   /* 空闲：四颗 LED 全灭并停在原地 */
    LED_FLOW_MODE_SINGLE = 1,   /* 逐个亮灭 */
    LED_FLOW_MODE_PAIR   = 2,   /* 两两亮灭 */
    LED_FLOW_MODE_ALL    = 3,   /* 一起亮灭 */
} led_flow_mode_t;

/**
 * @brief 单次触发软件定时器
 * @note  题目 5 要求 2：用 struct 记录起始时刻，封装 HAL_GetTick()。
 *        到期后自动停止，不重新启动不会再次触发（题目 5 要求 3）。
 */
typedef struct
{
    uint32_t start_ms;   /* 启动时刻，由 HAL_GetTick() 获取 */
    uint32_t period_ms;  /* 定时时长，单位 ms */
    uint8_t  running;    /* 运行标志：1 表示正在计时，0 表示已到期或停止 */
} soft_timer_t;

/* 状态机输入信号，由 main.c 定义，调试时手动修改观察模式切换 */
extern led_flow_mode_t signal;

/* 题目 5 要求 2：启动定时器 */
void timer_start(soft_timer_t *timer, uint32_t period_ms);

/* 题目 5 要求 2/3：查询定时器是否到期（单次触发） */
uint8_t timer_is_expired(soft_timer_t *timer);

/* 题目 5 要求 4：非阻塞状态机更新函数，每轮 while(1) 调用一次 */
void led_flow_update(void);

#endif /* LED_FLOW_H */
