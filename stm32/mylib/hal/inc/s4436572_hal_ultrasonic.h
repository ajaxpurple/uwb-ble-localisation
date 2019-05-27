/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_led.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   Ultrasonic HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_led_init() - Setup LED
 * s4436572_hal_led_deinit() - Deinitialise LED
 * s4436572_hal_led_toggle() - Toggle LED
 * s4436572_hal_led_write(value) - Set state of LED 
 ******************************************************************************
 */

#ifndef S4436572_HAL_ULTRASONIC_H
#define S4436572_HAL_ULTRASONIC_H
#include "stm32l4xx.h"

void s4436572_hal_ultrasonic_init();
uint16_t s4436572_hal_ultrasonic_read();
void s4436572_hal_ultrasonic_deinit();
#endif