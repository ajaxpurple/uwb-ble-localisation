/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_log.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   System log RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_log_init() - initialise system log
 * s4436572_os_log_deinit() - de-initialise system log
 * int s4436572_os_puts(char* string) - Task safe puts
 * int s4436572_os_log(MessageType msg_type, const char* format, ...) - Task safe log
 ******************************************************************************
 */

#ifndef S4436572_OS_LOG_H
#define S4436572_OS_LOG_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "s4436572_hal_printf.h"

typedef enum {
    NONE = 0,
    ERR = LIGHT_RED,
    LOG = LIGHT_GREEN,
    DEBUG = LIGHT_BLUE
}MessageType;

typedef struct{
	int Sequence_Number;
	char Payload[MESSAGE_LENGTH];
	MessageType MsgType;
}Message;

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

extern QueueHandle_t s4436572_QueueLog; //Printf queue
extern SemaphoreHandle_t s4436572_SemaphoreFilterLog;
extern SemaphoreHandle_t s4436572_SemaphoreFilterDebug;
extern SemaphoreHandle_t s4436572_SemaphoreFilterError;
extern SemaphoreHandle_t s4436572_SemaphoreFilterAll;

void s4436572_os_log_init(void); //Initialise system log
void s4436572_os_log_deinit(void); //De-initialise system log
int s4436572_os_puts(char* string);
int s4436572_os_log(MessageType msg_type, const char* format, ...);
#define console_log s4436572_os_log //myprintf define hook
#define console_puts s4436572_os_puts //myprintf define hook
#endif
