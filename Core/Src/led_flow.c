/**
 * @file    led_flow.c
 * @brief   流水灯模块实现。
 *          内部调用 led_on()/led_off() 实现单颗 LED 亮灭，
 *          main.c 通过 include led_flow.h 间接使用，看不到底层 LED 接口。
 */
#include "led_flow.h"
#include "led.h"            /* 仅在实现文件中包含底层 LED 驱动，main.c 不 include */

/**
 * @brief  控制单颗 LED 完成一次"亮-灭"
 * @param  step: 亮灭参数指针，包含 LED 编号、亮多久、灭多久
 * @note   本函数为模块内部函数（static），只在本文件使用，不对外暴露。
 *         题目 3 要求 2：实现 blink()，内部封装 led_on()/led_off() 和延时。
 */
static void blink(const led_step_config_t step)
{
    led_on(step.led_num);          /* 点亮指定 LED */
    HAL_Delay(step.on_ms);         /* 保持点亮指定时长 */
    led_off(step.led_num);         /* 熄灭指定 LED */
    HAL_Delay(step.off_ms);        /* 保持熄灭指定时长 */
}

/**
 * @brief  流水灯公共函数：让 4 颗 LED 依次亮灭
 * @note   题目 3 要求 4：将题目 2 的流水灯升级为调用 blink()。
 *         题目 3 要求 5：main.c 只调用本公共函数，不写流水灯逻辑。
 */

void waterfall_led0(void)
{
    /* 定义一次亮灭的参数结构体 */
    led_step_config_t step;

    step.on_ms  = 250U;     /* 每颗 LED 点亮 250 ms */
    step.off_ms = 250U;     /* 每颗 LED 熄灭 250 ms */

    /* 依次亮灭 */
    for (uint8_t i = 1U; i <= LED_FLOW_LED_COUNT; i++)
    {
        step.led_num = i;   /* 当前要操作的 LED 编号 */
        blink(step);       /* 调用内部 blink() 完成一次亮灭 */
    }
}

void waterfall_led1(void)
{

    /*两两亮灭*/
    for (uint8_t i = 1U; i <= LED_FLOW_LED_COUNT; i += 2)
    {
        led_on(i);
        led_on(i + 1);
        HAL_Delay(250U);
        led_off(i);
        led_off(i + 1);
        HAL_Delay(250U);

    }

}
void waterfall_led2(void)
{
    /*同时亮灭*/
    led_on(1);
    led_on(2);
    led_on(3);
    led_on(4);
    HAL_Delay(250U);
    led_off(1);
    led_off(2);
    led_off(3);
    led_off(4);
    HAL_Delay(250U);

}