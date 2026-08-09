/**
 * @file    led_flow.h
 * @brief   流水灯模块公共接口。
 *          本头文件只暴露结构体定义和公共流水灯函数，
 *          不暴露 led_on()/led_off() 等底层 LED 驱动接口。
 *          main.c 只 include 本头文件，即可实现"main 调不到底层 LED 函数"。
 */
#ifndef LED_FLOW_H
#define LED_FLOW_H

#include "stm32h7xx_hal.h"

#define LED_FLOW_LED_COUNT 4U   /* 流水灯使用的 LED 数量 */

/**
 * @brief 一次亮灭的参数结构体
 * @note  题目 3 要求 1：用结构体定义 LED 编号、亮多久、灭多久
 */
typedef struct
{
    uint8_t  led_num;   /* LED 编号，范围 1~LED_FLOW_LED_COUNT */
    uint16_t on_ms;     /* 点亮持续时间，单位 ms */
    uint16_t off_ms;    /* 熄灭持续时间，单位 ms */
} led_step_config_t;

/**
 * @brief 流水灯模式枚举
 * @note  题目 4 要求 1：用 enum 定义三种流水灯模式
 *        0：逐个亮灭，1：两两亮灭，2：一起亮灭
 */
typedef enum
{
    LED_FLOW_MODE_SINGLE = 0,   /* 逐个亮灭 */
    LED_FLOW_MODE_PAIR   = 1,   /* 两两亮灭 */
    LED_FLOW_MODE_ALL    = 2,   /* 一起亮灭 */
    LED_FLOW_MODE_MAX
} led_flow_mode_t;

/* 三种流水灯效果，分别对应枚举的三种模式 */
void waterfall_led0(void);  /* 模式 0：逐个亮灭 */
void waterfall_led1(void);  /* 模式 1：两两亮灭 */
void waterfall_led2(void);  /* 模式 2：一起亮灭（名字保持你原代码） */

#endif /* LED_FLOW_H */
