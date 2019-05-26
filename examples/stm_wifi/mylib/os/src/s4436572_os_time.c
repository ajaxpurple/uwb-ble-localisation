/**
 ******************************************************************************
 * @file    mylib/s4436572_os_time.c
 * @author  Rohan Malik � 44365721
 * @date    14032019
 * @brief   System timer RTOS
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_time_init() - initialise time
 * s4436572_os_time_deinit() - de-initialise time
 * s4436572_TaskTime() - Time task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_os_time.h"
#include "s4436572_os_log.h"
#include "stm32l4xx_hal.h"
#include <stdbool.h>

#define TICK_COUNT 20 //Waiting time for semaphores and queues

//Task configuration for time
#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define EXTRA_PRIORITY    2
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY

//Timer secondCount
TickType_t tickCount;
uint32_t secondCount;

//Task, queues and semaphores for time
TaskHandle_t s4436572_TaskHandleTime;
SemaphoreHandle_t s4436572_SemaphoreGetHMS;
SemaphoreHandle_t s4436572_SemaphoreGetSeconds;

void s4436572_TaskTime(void);

/**
  * @brief  Initialise time.
  * @param  None
  * @retval None
*/
extern void s4436572_os_time_init(void) {

	//Initialise queues and semaphores
	s4436572_SemaphoreGetSeconds = xSemaphoreCreateBinary();
  s4436572_SemaphoreGetHMS= xSemaphoreCreateBinary();


	//Start time tasks
	xTaskCreate( (TaskFunction_t) &s4436572_TaskTime, (const char *) "time", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleTime);
}


/**
  * @brief  De-initialise time.
  * @param  None
  * @retval None
*/
extern void s4436572_os_time_deinit(void) {

	//Delete time task
	vTaskDelete(s4436572_TaskHandleTime);

	//Delete semaphores and queues
	vSemaphoreDelete(s4436572_SemaphoreGetHMS);
  vSemaphoreDelete(s4436572_SemaphoreGetSeconds);
}

/**
  * @brief Time RTOS Task
  * @param  None
  * @retval None
*/
extern void s4436572_TaskTime(void){

	for(;;){
    int isHMS = 0;
    int isSeconds = 0;
    if(xSemaphoreTake(s4436572_SemaphoreGetHMS, TICK_COUNT) == pdTRUE){
        console_log(DEBUG, "Taking hms\r\n");
        isHMS = 1;
    }else if(xSemaphoreTake(s4436572_SemaphoreGetSeconds, TICK_COUNT) == pdTRUE){
        console_log(DEBUG, "Taking seconds\r\n");
        isSeconds = 1;
    }

    //Attempt to get system time secondCount
    if(isHMS || isSeconds){
        portENTER_CRITICAL();
        secondCount = HAL_GetTick()/1000;
        portEXIT_CRITICAL();
        if(isHMS){
          console_log(LOG, "%d hours, %d minutes, %d seconds\r\n", secondCount/60/60, (secondCount/60) % 60, secondCount % 60);
        }else{
          console_log(LOG, "%d seconds\r\n", secondCount);
        }
    }
    vTaskDelay(100);
	}
}
