
/**
  ******************************************************************************
  * @file    project2/main.c
  *  @author  Rohan Malik � 44365721
  * @date    17052018
  * @brief   Project 2
  ******************************************************************************
  *
  */

/* Includes. */
#include "FreeRTOS.h"
#include <stdio.h>
#include "stm32l4xx_hal.h"
#include "s4436572_hal_printf.h"
#include "s4436572_os_led.h"
#include "s4436572_os_time.h"
#include "s4436572_os_fatfs.h"
#include "s4436572_os_rec.h"
#include "s4436572_os_log.h"
#include "s4436572_os_cli.h"


#include "s4436572_cli_log.h"
#include "s4436572_cli_led.h"
#include "s4436572_cli_time.h"
#include "s4436572_cli_fatfs.h"
#include "s4436572_hal_led.h"
#include "task.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);

uint32_t count = 0;
uint32_t lastCount = 0;

void HardFault_Handler(){
	while(1){

	}
}

/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {
  HAL_Init();
  //s4436572_os_led_init(); //Initialise pantilt
	s4436572_os_fatfs_init(); //Initialise radio
	s4436572_os_rec_init(); //Initialise radio
	s4436572_os_log_init(); //Initialise printf function
	s4436572_os_cli_init(); //Start CLI task	

	s4436572_cli_fatfs_init();
	//s4436572_cli_led_init(); //CLI led
	//s4436572_cli_time_init(); //CLI system timer
	s4436572_cli_log_init(); //CLI log	

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
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