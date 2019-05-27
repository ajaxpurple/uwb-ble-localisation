/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_time.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   System timer RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_time_init() - initialise system timer
 * s4436572_os_time_deinit() - de-initialise system timer
 ******************************************************************************
 */

#ifndef S4436572_OS_TIME_H
#define S4436572_OS_TIME_H

#include "FreeRTOS.h"
#include "semphr.h"

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//Radio queue and semaphores
extern SemaphoreHandle_t s4436572_SemaphoreGetSeconds;
extern SemaphoreHandle_t s4436572_SemaphoreGetHMS;


void s4436572_os_time_init(void); //Initialise system timer
void s4436572_os_time_deinit(void); //De-initialise system timer

#endif
