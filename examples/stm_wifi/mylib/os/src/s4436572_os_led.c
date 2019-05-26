/**
 ******************************************************************************
 * @file    mylib/s4436572_os_led.c
 * @author  Rohan Malik - 44365721
 * @date    15032019
 * @brief   Led RTOS
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_led_init() - initialise led
 * s4436572_os_led_deinit() - de-initialise led
 * s4436572_TaskLed() - Led task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_os_led.h"
#include "s4436572_hal_led.h"
#include "s4436572_os_log.h"
#include "task.h"
#include "semphr.h"

#define TICK_COUNT 10 //Waiting time for semaphores and queues
#define LED_QUEUE_SIZE 10 //Queue size for led

#define TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE )
#define EXTRA_PRIORITY 		1
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Task, queues and semaphores for servo
TaskHandle_t s4436572_TaskHandleLed;
QueueHandle_t s4436572_QueueLedCommand;
SemaphoreHandle_t s4436572_SemaphoreLedToggleBlink;

void s4436572_TaskLed();

/**
  * @brief  Initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_led_init(void) {

	portDISABLE_INTERRUPTS();
	s4436572_hal_led_init();
	portENABLE_INTERRUPTS();

	//Initialise queues and semaphores
	s4436572_QueueLedCommand = xQueueCreate(LED_QUEUE_SIZE, sizeof(LedCommand));
	s4436572_SemaphoreLedToggleBlink = xSemaphoreCreateBinary();

	//Create task
	xTaskCreate( (TaskFunction_t) &s4436572_TaskLed, (const char *) "led", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleLed);

}


/**
  * @brief  De-initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_led_deinit(void) {

	//Delete task, queues and semaphore
	vTaskDelete(s4436572_TaskHandleLed);
	vQueueDelete(s4436572_QueueLedCommand);
	vSemaphoreDelete(s4436572_SemaphoreLedToggleBlink);

	portDISABLE_INTERRUPTS();
	s4436572_hal_led_deinit(); //Deinitialise led
	portENABLE_INTERRUPTS();

}

/**
  * @brief Led RTOS Task function
  * @param  None
  * @retval None
*/
extern void s4436572_TaskLed(void){
	LedCommand ledCommand;
	for(;;){
		if(xQueueReceive(s4436572_QueueLedCommand, &ledCommand, TICK_COUNT)){
			console_log(DEBUG, "Queue receive LED led=%d action=%d\r\n", ledCommand.led, ledCommand.action);
			if(ledCommand.action == TOGGLE){
				s4436572_hal_led_toggle(ledCommand.led - 1);
			}else{
				s4436572_hal_led_write(ledCommand.led - 1, ledCommand.action);
			}
		}
		vTaskDelay(200);
	}
}
