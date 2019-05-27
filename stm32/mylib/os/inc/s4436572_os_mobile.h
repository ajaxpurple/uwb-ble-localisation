/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_mobile.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Led RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_mobile_init() - initialise mobile
 * s4436572_os_mobile_deinit() - de-initialise mobile
 ******************************************************************************
 */

#ifndef S4436572_OS_MOBILE_H
#define S4436572_OS_MOBILE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


extern QueueHandle_t s4436572_QueueMobileSending;
/* External function prototypes -----------------------------------------------*/
void s4436572_os_mobile_init(void); //Initialise mobile
void s4436572_os_mobile_deinit(void); //De-initialise mobile

#endif
