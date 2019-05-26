

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include <string.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "s4436572_hal_printf.h"
#include "s4436572_hal_led.h"
#include "s4436572_hal_imu.h"
#include "s4436572_hal_sysclk.h"
#include "s4436572_hal_mouse.h"
#include "s4436572_hal_ultrasonic.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_gyro.h"

#include "s4436572_os_log.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_led.h"
#include "s4436572_os_wifi.h"
#include "s4436572_os_ble.h"
#include "s4436572_os_mobile.h"

#include "task.h"
#include "lsm6dsl.h"

#include "s4436572_cli_log.h"
#include "s4436572_cli_ble.h"
#include "s4436572_cli_wifi.h"



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);

#define TASK_STACK_SIZE (configMINIMAL_STACK_SIZE*2)
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
  s4436572_os_wifi_init();
  
  s4436572_os_mobile_init();
  s4436572_os_cli_init(); //Start CLI task	
  
  s4436572_cli_log_init(); //CLI log
  s4436572_cli_ble_init(); //CLI Bluetooth
  s4436572_cli_wifi_init(); //CLI Wifi
  
  xTaskCreate((TaskFunction_t) &Blink_Task, (const char *) "blink", TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &taskBlinkHandle);
  vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}

void Blink_Task(void){
  //BSP_MAGNETO_Init();
  //BSP_GYRO_Init();
  //int16_t axes[3];
  //int16_t angle;
  //float gyroData[3] = {0};
  //int16_t gyroAngle;

  for(;;){
    s4436572_hal_led_toggle(0);
    //BSP_MAGNETO_GetXYZ(axes);
    //angle = round(180*atan2(axes[1], axes[0])/3.14159265);
    //console_log(LOG, "X: %d, Y: %d, Z: %d, Angle: %d\r\n", axes[0], axes[1], axes[2], angle);
    //BSP_GYRO_GetXYZ(gyroData);
    //gyroAngle = round(process_gyro_data(gyroData));
    //console_log(LOG, "Gyro: %d\r\n", gyroAngle);
    vTaskDelay(300);
  }
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

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}