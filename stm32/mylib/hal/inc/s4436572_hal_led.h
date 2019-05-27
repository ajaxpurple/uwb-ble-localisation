/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_led.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   LED HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_led_init() - Setup LED
 * s4436572_hal_led_deinit() - Deinitialise LED
 * s4436572_hal_led_toggle() - Toggle LED
 * s4436572_hal_led_write(value) - Set state of LED 
 ******************************************************************************
 */

#ifndef S4436572_HAL_LED_H
#define S4436572_HAL_LED_H

typedef enum{
    LED_GREEN1,
    LED_GREEN2,
    LED_WIFI,
    LED_BLE,
    LED_ERR
}LedType;

void s4436572_hal_led_init();
void s4436572_hal_led_toggle(int led);
void s4436572_hal_led_write(int led, int value);
void s4436572_hal_led_deinit();
#endif