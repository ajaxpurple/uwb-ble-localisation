

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include <stdio.h>
#include "stm32l4xx.h"
#include "s4436572_hal_printf.h"
#include "s4436572_hal_led.h"
#include "s4436572_os_ble.h"
#include "s4436572_hal_imu.h"
#include "s4436572_hal_sysclk.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "s4436572_os_log.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_led.h"
#include "task.h"


#include "s4436572_cli_log.h"
#include "s4436572_cli_ble.h"

#define STM32_ID 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);

#define TASK_STACK_SIZE (configMINIMAL_STACK_SIZE*3)
void Hardware_init(void);
void Blink_Task(void);

uint32_t count = 0;
uint32_t lastCount = 0;
TaskHandle_t taskBlinkHandle;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();

  s4436572_os_led_init(); 
  s4436572_os_ble_init(); 
  s4436572_os_log_init(); //Initialise printf function
  
  s4436572_os_cli_init(); //Start CLI task	

  s4436572_cli_log_init(); //CLI log
  s4436572_cli_ble_init(); //CLI Bluetooth
  
  xTaskCreate((TaskFunction_t) &Blink_Task, (const char *) "blink", TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &taskBlinkHandle);
  vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}

void Blink_Task(void){
  BLEPacket packet;
	uint8_t device_id = STM32_ID;
	uint8_t node_id = 0;
  xSemaphoreGive(s4436572_SemaphoreBLEAdvertiseStart);
  for(;;){
    s4436572_hal_led_toggle(0);
  	packet.major = node_id;
	  packet.minor = device_id;
	  xQueueSend(s4436572_QueueBLEAdvertise, &packet, 10);
      vTaskDelay(100);
  }
  /* USER CODE END 3 */
}
void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
  
  for( ;; ){
		console_log(ERR, "Out of memory\r\n");
		HAL_Delay(200);
  }
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	set_color(ERR);
	for( ;; ){
		debug_printf("ERROR: Stack overflow of task %s at address %p\r\n", pcTaskName, (void*) pxTask);
		console_log(ERR, "Stack overflow of task %s at address %p\n", pcTaskName, (void*) pxTask);
		HAL_Delay(200);
	}
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	volatile size_t xFreeStackSpace;
	
	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amout of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();
	if( xFreeStackSpace > 100 ){
		if(count - lastCount > 100){
			lastCount = count;
			//pvPortMalloc(500);
			if(count < 10000){
				//console_log(DEBUG, "Time: %dms, Memory: %d\r\n", count, xFreeStackSpace);
			}
		}
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}

void vApplicationTickHook( void ){
	HAL_IncTick();
	count++;
}