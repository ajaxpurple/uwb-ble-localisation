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
#include "s4436572_os_wifi.h"
#include "s4436572_hal_wifi.h"
#include "s4436572_hal_led.h"
#include "s4436572_os_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define WIFI_QUEUE_SIZE 20 //Log queue size

//Task stack size and priority
#define EXTRA_PRIORITY 		2
#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 15 )
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Mylog task handle, queue, semaphore and message struct
TaskHandle_t s4436572_TaskHandleWifi;
QueueHandle_t s4436572_QueueWifiNetworkConfig;
QueueHandle_t s4436572_QueueWifiConnectionConfig;
QueueHandle_t s4436572_QueueWifiSend; //Wifi send queue
QueueHandle_t s4436572_QueueWifiReceive; //Wifi receive queue

SemaphoreHandle_t s4436572_SemaphoreWifiAccelToggle;
SemaphoreHandle_t s4436572_SemaphoreWifiOff;
SemaphoreHandle_t s4436572_SemaphoreWifiScan;
SemaphoreHandle_t s4436572_SemaphoreWifiJoin;
SemaphoreHandle_t s4436572_SemaphoreWifiConnect;
SemaphoreHandle_t s4436572_SemaphoreWifiSuccess;
SemaphoreHandle_t s4436572_SemaphoreWifiSent;
SemaphoreHandle_t s4436572_SemaphoreWifiReceiveOn;
SemaphoreHandle_t s4436572_SemaphoreWifiReceiveOff;

WifiPacket packet;
Connection connection;
NetworkConfig config;

void s4436572_TaskWifi(void);

/**
  * @brief Mylog initialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_wifi_init(void){
	//Create the log queue and semaphore

	s4436572_QueueWifiNetworkConfig = xQueueCreate(1, sizeof(NetworkConfig));
	s4436572_QueueWifiConnectionConfig = xQueueCreate(1, sizeof(Connection));
	s4436572_QueueWifiSend = xQueueCreate(WIFI_QUEUE_SIZE, sizeof(WifiPacket));
	s4436572_QueueWifiReceive = xQueueCreate(WIFI_QUEUE_SIZE, sizeof(WifiPacket));

	s4436572_SemaphoreWifiOff = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiScan = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiJoin = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiConnect = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiSuccess = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiSent = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiReceiveOn = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiReceiveOff = xSemaphoreCreateBinary();
	s4436572_SemaphoreWifiAccelToggle = xSemaphoreCreateBinary();

	xTaskCreate( (TaskFunction_t) &s4436572_TaskWifi, (const char *) "wifi",
			TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleWifi);

}

/**
  * @brief Mylog deinitialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_wifi_deinit(void){

	//Delete the task
	vTaskDelete(s4436572_TaskHandleWifi);

	//Delete the log queue and semaphore
	vQueueDelete(s4436572_QueueWifiNetworkConfig);
	vQueueDelete(s4436572_QueueWifiConnectionConfig);
	vQueueDelete(s4436572_QueueWifiSend); //Wifi send queue
	vQueueDelete(s4436572_QueueWifiReceive); //Wifi receive queue

	vSemaphoreDelete(s4436572_SemaphoreWifiOff);
	vSemaphoreDelete(s4436572_SemaphoreWifiScan);
	vSemaphoreDelete(s4436572_SemaphoreWifiJoin);
	vSemaphoreDelete(s4436572_SemaphoreWifiConnect);
	vSemaphoreDelete(s4436572_SemaphoreWifiSent);
	vSemaphoreDelete(s4436572_SemaphoreWifiReceiveOn);
	vSemaphoreDelete(s4436572_SemaphoreWifiReceiveOff);
	vSemaphoreDelete(s4436572_SemaphoreWifiAccelToggle);
}

/**
  * @brief BLE task - task that handles bluetooth scanning and receiving
  * @param  None
  * @retval None
*/
extern void s4436572_TaskWifi(void){
	int status = s4436572_hal_wifi_init();
	
	if(status != 0){
		console_log(ERR, "Failed to initialise wifi\r\n");
	}else{
		console_log(DEBUG, "Initialised wifi\r\n");
	}
	
	WifiPacket packet;
	ScanResult result;
	bool joined = false;
	bool connected = false;
	int socket = 0;
	

	for(;;){
		if(xSemaphoreTake(s4436572_SemaphoreWifiScan, 0)){
			console_log(LOG, "Scanning...\r\n");			
			result = s4436572_hal_wifi_scan();
			for(uint8_t i=0; i<result.count; i++){
				console_log(LOG, "SSID: %s, RSSI: %d\r\n", result.ap[i].SSID, result.ap[i].RSSI);
			}
			if(result.count == 0){
				console_log(LOG, "No networks found\r\n");
			}
		}
		
		if(xQueueReceive(s4436572_QueueWifiNetworkConfig, &config, 0)){
			console_log(LOG, "Configure: SSID %s, Password %s\r\n", config.SSID, config.password);
		}
		if(xQueueReceive(s4436572_QueueWifiConnectionConfig, &connection, 0)){
			console_log(LOG, "Connection configuration: IP %d.%d.%d.%d, Port %d\r\n", connection.address[0], connection.address[1], connection.address[2], connection.address[3], connection.port);
		}

		if(xSemaphoreTake(s4436572_SemaphoreWifiJoin, 0)){
			console_log(LOG, "Joining network %s...\r\n", config.SSID);
			int status = s4436572_hal_wifi_join(config.SSID, config.password);
			while(status != 0){
				s4436572_hal_led_write(LED_ERR, 1);
				console_log(ERR, "Failed to join %s, error=%d\r\n", config.SSID, status);
				vTaskDelay(100);
				s4436572_hal_wifi_init();
				status = s4436572_hal_wifi_join(config.SSID, config.password);
				joined = false;
			}
			s4436572_hal_led_write(LED_WIFI, 1);
			console_log(LOG, "Joined network %s\r\n", config.SSID);
			joined = true;
		}
		if(joined && xSemaphoreTake(s4436572_SemaphoreWifiConnect, 0)){
			console_log(LOG, "Connecting to %d.%d.%d.%d:%d...\r\n",connection.address[0], connection.address[1], connection.address[2], connection.address[3], connection.port);
			int status = s4436572_hal_wifi_open(socket,"Hello", connection.address, connection.port);
			if(status != 0){
				s4436572_hal_led_write(LED_ERR, 1);
				console_log(ERR, "Failed to connect to server, error=%d\r\n", status);
				connected = false;
				joined = false;
				s4436572_hal_wifi_close(socket);				
				xSemaphoreGive(s4436572_SemaphoreWifiJoin);
				xSemaphoreGive(s4436572_SemaphoreWifiConnect);
			}else{
				s4436572_hal_led_write(LED_WIFI, 1);
				console_log(LOG, "Connected to %d.%d.%d.%d:%d\r\n", connection.address[0], connection.address[1], connection.address[2], connection.address[3], connection.port);
				xSemaphoreGive(s4436572_SemaphoreWifiSuccess);
				connected = true;
			}
		}
		if(connected && xQueueReceive(s4436572_QueueWifiSend, &packet, 0)){
			int status = s4436572_hal_wifi_write(socket, (uint8_t*) packet.data, packet.length);
			if(status == packet.length){
				console_log(LOG, "%s\r\n", packet.data);
				s4436572_hal_led_write(LED_WIFI, 1);
				xSemaphoreGive(s4436572_SemaphoreWifiSent);			
			}else{
				
				if(s4436572_hal_wifi_ping(connection.address) != 0){
					connected = false;
					xSemaphoreGive(s4436572_SemaphoreWifiConnect);
				}
				console_log(ERR, "Failed to send packet %s\r\n", packet.data);
				xQueueSendToFront(s4436572_QueueWifiSend, &packet, 0);
			}
		}

		if(xSemaphoreTake(s4436572_SemaphoreWifiReceiveOn, 0)){
			uint8_t len;
			do{
				len = s4436572_hal_wifi_read(socket, (uint8_t*) packet.data, 20);
				if(len > 0){
					packet.length = len;
					console_log(LOG, "Received packet %s\r\n", packet.data);
					s4436572_hal_led_write(LED_WIFI, 1);
				}
			}while(len > 0);
		}
		vTaskDelay(10);
		s4436572_hal_led_write(LED_WIFI, 0);
		s4436572_hal_led_write(LED_ERR, 0);
	}
}
