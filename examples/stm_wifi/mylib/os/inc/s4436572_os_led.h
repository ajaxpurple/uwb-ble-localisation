/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_led.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Led RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_led_init() - initialise led
 * s4436572_os_led_deinit() - de-initialise led
 ******************************************************************************
 */

#ifndef S4436572_OS_LED_H
#define S4436572_OS_LED_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "s4436572_hal_led.h"

typedef enum {
    ON = 1,
    OFF = 0,
    TOGGLE = 2
}LedAction;

typedef struct{
	LedType led;
	LedAction action;
}LedCommand;

extern QueueHandle_t s4436572_QueueLedCommand;
extern SemaphoreHandle_t s4436572_SemaphoreLedToggleBlink;

/* External function prototypes -----------------------------------------------*/
void s4436572_os_led_init(void); //Initialise led
void s4436572_os_led_deinit(void); //De-initialise led

#endif
