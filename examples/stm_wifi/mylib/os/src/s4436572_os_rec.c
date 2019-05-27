/**
 ******************************************************************************
 * @file    mylib/s4436572_os_rec.c
 * @author  Rohan Malik - 44365721
 * @date    15032019
 * @brief   Recording RTOS
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_rec_init() - initialise recording
 * s4436572_os_rec_deinit() - de-initialise recording
 * s4436572_TaskRec() - Recording task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_os_rec.h"
#include "s4436572_hal_imu.h"
#include "s4436572_os_log.h"
#include "s4436572_os_fatfs.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include <string.h>

#define TICK_COUNT 10 //Waiting time for semaphores and queues
#define REC_QUEUE_SIZE 10 //Queue size for led

#define TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE * 5)
#define EXTRA_PRIORITY 		1
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Task, queues and semaphores for servo
TaskHandle_t s4436572_TaskHandleRec;
QueueHandle_t s4436572_QueueRecCommand;

void s4436572_TaskRec();

/**
  * @brief  Initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_rec_init(void) {

	portDISABLE_INTERRUPTS();
	s4436572_hal_imu_init();
	portENABLE_INTERRUPTS();

	//Initialise queues and semaphores
	s4436572_QueueRecCommand = xQueueCreate(REC_QUEUE_SIZE, sizeof(RecCommand));
	
	//Create task
	xTaskCreate( (TaskFunction_t) &s4436572_TaskRec, (const char *) "rec", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleRec);

}


/**
  * @brief  De-initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_rec_deinit(void) {

	//Delete task, queues and semaphore
	vTaskDelete(s4436572_TaskHandleRec);
	vQueueDelete(s4436572_QueueRecCommand);

	portDISABLE_INTERRUPTS();
	s4436572_hal_imu_deinit(); //Deinitialise led
	portENABLE_INTERRUPTS();

}

/**
  * @brief Led RTOS Task function
  * @param  None
  * @retval None
*/
extern void s4436572_TaskRec(void){
	XYZValues accelValues, gyroValues, magnetValues;
	RecConfig config = {0};
	FatfsCommand accelDiskOp, gyroDiskOp, magnetDiskOp;
	RecCommand recCommand;
	TickType_t current_time1;
	TickType_t current_time2;
	TickType_t current_time3;
	TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
	for(;;){
		current_time1 = xTaskGetTickCount();
		if(config.accel.enabled){
			accelValues = s4436572_hal_accel_read();
			accelDiskOp.action = WRITE_FILE;
			strcpy(accelDiskOp.name, config.accel.filename);
			memset(accelDiskOp.name2, 0, 80);
			sprintf(accelDiskOp.name2, "{type: \"accel\", time: %lu, x:%d, y:%d, z:%d}\r\n", current_time1, accelValues.x, accelValues.y, accelValues.z);
			//console_log(LOG, "%s", accelDiskOp.name2);
			xQueueSend(s4436572_QueueFatfsCommand, &accelDiskOp, 0);
		}
		
		
		if(config.gyro.enabled){
			gyroValues = s4436572_hal_gyro_read();
			gyroDiskOp.action = WRITE_FILE;
			current_time2 = xTaskGetTickCount();
			strcpy(gyroDiskOp.name, config.gyro.filename);
			memset(gyroDiskOp.name2, 0, 80);
			sprintf(gyroDiskOp.name2, "{type: \"gyro\", time: %lu, x:%d, y:%d, z:%d}\r\n", current_time2, gyroValues.x, gyroValues.y, gyroValues.z);
			//console_log(LOG, "%s", gyroDiskOp.name2);
			xQueueSend(s4436572_QueueFatfsCommand, &gyroDiskOp, 0);
		}
		
		current_time3 = xTaskGetTickCount();
		if(config.magnet.enabled){
			magnetValues = s4436572_hal_magnet_read();
			magnetDiskOp.action = WRITE_FILE;
			strcpy(magnetDiskOp.name, config.magnet.filename);
			memset(magnetDiskOp.name2, 0, 80);
			sprintf(magnetDiskOp.name2, "{type: \"magnet\", time: %lu, x:%d, y:%d, z:%d}\r\n", current_time3, magnetValues.x, magnetValues.y, magnetValues.z);
			//console_log(LOG, "%s", magnetDiskOp.name2);
			xQueueSend(s4436572_QueueFatfsCommand, &magnetDiskOp, 0);
		}

		if(xQueueReceive(s4436572_QueueRecCommand, &recCommand, 0)){
			console_log(DEBUG, "Queue receive rec\r\n");
			switch(recCommand.sensorType){
				case ACCEL:
					config.accel = recCommand.setting;
					if(config.accel.enabled){
						console_log(LOG, "Logging %s sensor at %dHz to file: %s\r\n", "accelerometer", 10, recCommand.setting.filename);
					}else{
						accelDiskOp.action = CLOSE_FILE;
						console_log(LOG, "Stopped logging of %s\r\n", "accelerometer");
						xQueueSend(s4436572_QueueFatfsCommand, &accelDiskOp, 0);
					}
					break;
				case GYRO:
					config.gyro = recCommand.setting;
					if(config.gyro.enabled){
						console_log(LOG, "Logging %s sensor at %dHz to file: %s\r\n", "gyroscope", 10, recCommand.setting.filename);
					}else{
						gyroDiskOp.action = CLOSE_FILE;
						console_log(LOG, "Stopped logging of %s\r\n", "gyroscope");
						xQueueSend(s4436572_QueueFatfsCommand, &gyroDiskOp, 0);
					}
					break;
				case MAGNET:
					config.magnet = recCommand.setting;
					if(config.magnet.enabled){
						console_log(LOG, "Logging %s sensor at %dHz to file: %s\r\n", "magnetometer", 10, recCommand.setting.filename);
					}else{
						magnetDiskOp.action = CLOSE_FILE;
						console_log(LOG, "Stopped logging of %s\r\n", "magnetometer");
						xQueueSend(s4436572_QueueFatfsCommand, &magnetDiskOp, 0);
					}
					break;
			}		
		}
		vTaskDelayUntil(&xLastWakeTime, 100);
	}
}
