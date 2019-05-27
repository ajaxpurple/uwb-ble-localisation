/**
 ******************************************************************************
 * @file    mylib/s4436572_os_log.c
 * @author  Rohan Malik � 44365721
 * @date    17032019
 * @brief   RTOS Task safe log wrapper
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_print(const char* format, ...) - Task safe log
 * s4436572_os_log_init() - initialise system log
 * s4436572_os_log_deinit() - de-initialise system log
 * int s4436572_os_puts(char* string) - Task safe puts
 * int s4436572_os_log(MessageType msg_type, const char* format, ...) - Task safe log
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4436572_os_ble.h"
#include "s4436572_hal_ble.h"
#include "s4436572_os_log.h"
#include "s4436572_hal_led.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BLE_QUEUE_SIZE 10 //Log queue size

//Task stack size and priority
#define EXTRA_PRIORITY 		2
#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 5 )
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Mylog task handle, queue, semaphore and message struct
TaskHandle_t s4436572_TaskHandleBLE;
QueueHandle_t s4436572_QueueBLEAdvertise = NULL;
QueueHandle_t s4436572_QueueBLEReceive = NULL;
QueueHandle_t s4436572_QueueBLEBeacon = NULL;
QueueHandle_t s4436572_QueueBLEFilterAddress = NULL;
SemaphoreHandle_t s4436572_SemaphoreBLEFilterOn;
SemaphoreHandle_t s4436572_SemaphoreBLEFilterOff;
SemaphoreHandle_t s4436572_SemaphoreBLEScanStart;
SemaphoreHandle_t s4436572_SemaphoreBLEScanStop;
SemaphoreHandle_t s4436572_SemaphoreBLEAccelToggle;
SemaphoreHandle_t s4436572_SemaphoreBLEAdvertiseStop;
SemaphoreHandle_t s4436572_SemaphoreBLEAdvertiseStart;

BLEPacket blePacket;

uint8_t sendingUUID[] = {
	0x43, 0x98, 0x17, 0x22, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t serverAddr[] = {0x00, 0x00, 0x22, 0x17, 0x98, 0x43};
IBeaconData beaconData;

uint8_t addrFilter[] = {0x26, 0x84, 0x33, 0x44, 0x00, 0x00};
bool filter = false;
BLEFilter bleFilter;

void s4436572_TaskBLE(void);

/**
  * @brief Mylog initialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_ble_init(void){
	//Create the log queue and semaphore
	s4436572_QueueBLEAdvertise = xQueueCreate(BLE_QUEUE_SIZE, sizeof(BLEPacket));
	s4436572_QueueBLEFilterAddress = xQueueCreate(1, sizeof(BLEFilter));
	s4436572_QueueBLEReceive = xQueueCreate(BLE_QUEUE_SIZE, sizeof(Advertisement));
	s4436572_QueueBLEBeacon = xQueueCreate(BLE_QUEUE_SIZE, sizeof(IBeaconData));
	s4436572_SemaphoreBLEScanStart = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEScanStop = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEFilterOn = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEFilterOff = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEAccelToggle = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEAdvertiseStop = xSemaphoreCreateBinary();
	s4436572_SemaphoreBLEAdvertiseStart = xSemaphoreCreateBinary();

	xTaskCreate( (TaskFunction_t) &s4436572_TaskBLE, (const char *) "ble",
			TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleBLE);

}

/**
  * @brief Mylog deinitialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_ble_deinit(void){

	//Delete the task
	vTaskDelete(s4436572_TaskHandleBLE);

	//Delete the log queue and semaphore
	vQueueDelete(s4436572_QueueBLEAdvertise);
	vQueueDelete(s4436572_QueueBLEReceive);
	vQueueDelete(s4436572_QueueBLEBeacon);
	vQueueDelete(s4436572_QueueBLEFilterAddress);
	vSemaphoreDelete(s4436572_SemaphoreBLEScanStart);
	vSemaphoreDelete(s4436572_SemaphoreBLEScanStop);
	vSemaphoreDelete(s4436572_SemaphoreBLEFilterOff);
	vSemaphoreDelete(s4436572_SemaphoreBLEFilterOn);
	vSemaphoreDelete(s4436572_SemaphoreBLEAdvertiseStop);
	vSemaphoreDelete(s4436572_SemaphoreBLEAdvertiseStart);
}

void scan_result(Advertisement adv){

	if(!filter || memcmp(addrFilter, adv.address, 6) == 0){
		xQueueSend(s4436572_QueueBLEReceive, &adv, 0);
	}
}

uint8_t parse_hex_digit(uint8_t letter){
    if(letter >='0' && letter <= '9'){
        return letter - '0';
    }else if(letter >= 'A' && letter <= 'F'){
        return (letter - 'A') + 10;
    }else if(letter>='a' && letter <='f'){
        return (letter - 'a') + 10;
    }else{
        return 0;
    }
}

/**
  * @brief BLE task - task that handles bluetooth scanning and receiving
  * @param  None
  * @retval None
*/
extern void s4436572_TaskBLE(void){
	s4436572_hal_ble_init(serverAddr);
	s4436572_hal_ble_set_uuid(sendingUUID);

	BLEPacket recvPacket;
	Advertisement advert;
	char line[100];
	bool scan = false;

	for(;;){
		if(scan){
			s4436572_hal_ble_check();
		}
		if(xSemaphoreTake(s4436572_SemaphoreBLEScanStart, 0)){
			//Scan for bluetooth advertisements
			console_log(LOG, "Scanning...\r\n");
			s4436572_hal_ble_scan_start(&scan_result);
			scan = true;
		}else if(xSemaphoreTake(s4436572_SemaphoreBLEScanStop, 0)){
			s4436572_hal_ble_scan_stop();
			console_log(LOG, "Scan stopped\r\n");
			s4436572_hal_led_write(LED_BLE, 0);
			scan = false;
		}else if(xSemaphoreTake(s4436572_SemaphoreBLEAdvertiseStop, 0)){
			s4436572_hal_ble_advertise_stop();
			s4436572_hal_led_write(LED_BLE, 0);
			console_log(LOG, "Stopped discovery\r\n");
		}else if(xSemaphoreTake(s4436572_SemaphoreBLEAdvertiseStart, 0)){
			s4436572_hal_ble_advertise_start();
			console_log(LOG, "Started discovery\r\n");
		}else if(xQueueReceive(s4436572_QueueBLEAdvertise, &recvPacket, 0)){
			s4436572_hal_ble_advertise(recvPacket.major, recvPacket.minor);
			s4436572_hal_led_write(LED_BLE, 1);
			console_log(DEBUG, "Sent packet with major=%d, minor=%d\r\n", recvPacket.major, recvPacket.minor);
		}else if(xQueueReceive(s4436572_QueueBLEFilterAddress, &bleFilter, 0)){
			console_log(DEBUG, "Queue receive filter address\r\n");			        
			for(uint8_t i=0; i<6; i++){
				addrFilter[i] = (parse_hex_digit(bleFilter.address[3*i])<<4) | parse_hex_digit(bleFilter.address[3*i+1]);
			}
			console_log(DEBUG, "Filter address set to %02x:%02x:%02x:%02x:%02x:%02x\r\n", 
				addrFilter[0], addrFilter[1], addrFilter[2], addrFilter[3], addrFilter[4],addrFilter[5]);
		}else if(xSemaphoreTake(s4436572_SemaphoreBLEFilterOn, 0)){
			filter = true;
		}else if(xSemaphoreTake(s4436572_SemaphoreBLEFilterOff, 0)){
			filter = false;
		}else if(xQueueReceive(s4436572_QueueBLEReceive, &advert, 10) == pdTRUE){
				s4436572_hal_led_write(LED_BLE, 1);				
				//s4436572_os_log(DEBUG, "Advertisement from %02x:%02x:%02x:%02x:%02x:%02x with packet(%d):", 
				//advert.address[0], advert.address[1], advert.address[2], advert.address[3], advert.address[4], advert.address[5], advert.length);
				//for(uint8_t i=0; i<advert.length; i++){
			//		s4436572_os_log(DEBUG, "%02x ", advert.data[i]);
			//	}
				//s4436572_os_log(DEBUG, "\r\n");
				
				if(extract_ibeacon_data(advert, &beaconData)){
					sprintf(line, "IBeacon UUID: ");
					for(uint8_t i=0; i<16; i++){
						sprintf(line + 14 + 2*i, "%02x", beaconData.uuid[i]);
					}
					sprintf(line + 14 + 31, ", Major: %d, Minor: %d, RSSI at 1m: %d", beaconData.major, beaconData.minor, beaconData.rssiAt1m);
					s4436572_os_log(LOG, "%s\r\n", line);
					xQueueSend(s4436572_QueueBLEBeacon, &beaconData, 0);
				}
		}
		vTaskDelay(100);
		s4436572_hal_led_write(LED_BLE, 0);
	}
}
