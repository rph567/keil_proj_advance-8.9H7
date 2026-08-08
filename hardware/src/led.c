/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"

inline uint8_t check(uint8_t led_num)
{
    if (led_num < 1 || led_num > 4)
    {
        return 0; // 无效的 LED 编号
    }
    return 1; // 有效的 LED 编号
}
/* 点亮指定 LED */
void led_on(uint8_t led_num)
{
    if (!check(led_num))
    {
        return; // 无效的 LED 编号
    }

    switch (led_num)
    {
        case 1:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
            break;
        default:

            break;
    }
}

/* 熄灭指定 LED */
void led_off(uint8_t led_num)
{    switch (led_num)
    {
        case 1:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET);
            break;
        case 4:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
void waterfall_led(uint8_t led_num)
{


    for (uint8_t i = 1; i <= led_num; i++)
    {
        led_on(i);
        HAL_Delay(250U);
        led_off(i);
    }
}