/**
 ******************************************************************************
 * @file    mylib/s4436572_os_mobile.c
 * @author  Rohan Malik - 44365721
 * @date    15032019
 * @brief   Mobile node RTOS
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_mobile_init() - initialise mobile
 * s4436572_os_mobile_deinit() - de-initialise mobile
 * s4436572_TaskMobile() - Mobile task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_os_wifi.h"
#include "s4436572_os_ble.h"
#include "s4436572_os_log.h"
#include "s4436572_os_mobile.h"
#include "s4436572_hal_ble.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"
#include "task.h"
#include "semphr.h"
#include "stdbool.h"
#include "string.h"
#include <stdio.h>
#include <math.h>

#define TICK_COUNT 10 //Waiting time for semaphores and queues
#define MOBILE_QUEUE_SIZE 20 //Queue size

#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 5 )
#define EXTRA_PRIORITY      2
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Task, queues and semaphores for servo
TaskHandle_t s4436572_TaskHandleMobile;
QueueHandle_t s4436572_QueueMobileSending;
uint8_t steps;
uint8_t old_steps;


void s4436572_TaskMobile();

typedef struct{
    int8_t rssi;
    uint8_t id;
    uint16_t major;
    uint16_t minor;
}BLENode;

typedef struct{
    BLENode nodes[4];
    bool exists[4];
    int16_t heading;
    uint8_t step;
}WifiNode;

/**
  * @brief  Initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_mobile_init(void) {
	//Initialise queues and semaphores
	s4436572_QueueMobileSending = xQueueCreate(MOBILE_QUEUE_SIZE, sizeof(WifiNode));
	
	//Create task
	xTaskCreate( (TaskFunction_t) &s4436572_TaskMobile, (const char *) "mobile", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleMobile);
}


/**
  * @brief  De-initialise led.
  * @param  None
  * @retval None
*/
extern void s4436572_os_mobile_deinit(void) {
	//Delete task, queues and semaphore
	vTaskDelete(s4436572_TaskHandleMobile);
	vQueueDelete(s4436572_QueueMobileSending);
}

int8_t findId(uint8_t* uuid){
    uint8_t UUID1[] = {
        0x43, 0x98, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t UUID2[] = {
        0x43, 0x98, 0x17, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t UUID3[] = {
        0x43, 0x98, 0x17, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t UUID4[] = {
        0x43, 0x98, 0x17, 0x03, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    if(memcmp(uuid, UUID1, 16) == 0){
        return 0;
    }else if(memcmp(uuid, UUID2, 16) == 0){
        return 1;
    }else if(memcmp(uuid, UUID3, 16) == 0){
        return 2;
    }else if(memcmp(uuid, UUID4, 16) == 0){
        return 3;
    }else{
        return -1;
    }
}

/**
  * @brief Led RTOS Task function
  * @param  None
  * @retval None
*/
extern void s4436572_TaskMobile(void){
    NetworkConfig config;
    BLENode bleNode;
    WifiNode wifiNode;
    BLEPacket syncPacket;
    IBeaconData recvBeacon;
    WifiPacket wPacket;
    int16_t axes[3];
    int16_t angle;
    BSP_GYRO_Init();
    BSP_ACCELERO_Init();
    BSP_MAGNETO_Init();
    char serialData[110];

    bool allExist = false;
    for(uint8_t i=0; i<4; i++){
        wifiNode.exists[i] = false;
    }

    strcpy(config.SSID, "ajaxpurple");
    strcpy(config.password, "4011prac3");
    //strcpy(config.SSID, "iiNet1B9331");
    //strcpy(config.password, "adsl1965");
    xSemaphoreGive(s4436572_SemaphoreBLEAdvertiseStart);
    syncPacket.major = 0;
    syncPacket.minor = 0;
    xQueueSend(s4436572_QueueBLEAdvertise, &syncPacket, 0);
    vTaskDelay(300);
    xQueueSend(s4436572_QueueWifiNetworkConfig, &config, 50);

    Connection conn;
    //uint8_t address[] =  {192, 168, 43, 36};
    uint8_t address[] =  {192, 168, 43, 253};
    //uint8_t address[] =  {192, 168, 0, 5};
    //uint8_t address[] =  {10, 1, 1, 246};
    memcpy(conn.address, address, 4);
    conn.port = 65432;
    xQueueSend(s4436572_QueueWifiConnectionConfig, &conn, 50);
    xSemaphoreGive(s4436572_SemaphoreWifiJoin);
    xSemaphoreGive(s4436572_SemaphoreWifiConnect);

    if(xSemaphoreTake(s4436572_SemaphoreWifiSuccess, portMAX_DELAY)){
        xSemaphoreGive(s4436572_SemaphoreBLEScanStart);
    }

	for(;;){
        if(xQueueReceive(s4436572_QueueBLEBeacon, &recvBeacon, TICK_COUNT)){
            //console_log(DEBUG, "Queue receive ble\r\n");
            bleNode.rssi = recvBeacon.rssi;
            bleNode.id = findId(recvBeacon.uuid);
            if(bleNode.id >= 0 && bleNode.id < 4){
                s4436572_os_log(LOG, "RSSI: %d, IBeacon UUID: ", bleNode.rssi);
                for(uint8_t i=0; i<16; i++){
                    s4436572_os_log(LOG, "%02x", recvBeacon.uuid[i]);
                }
                s4436572_os_log(LOG, ", Major: %d, Minor: %d, RSSI at 1m: %d\r\n", recvBeacon.major, recvBeacon.minor, recvBeacon.rssiAt1m);
                bleNode.major = recvBeacon.major;
                bleNode.minor = recvBeacon.minor;
                wifiNode.nodes[bleNode.id] = bleNode;
                wifiNode.exists[bleNode.id] = true;

                allExist = true;
                for(uint8_t i=0; i<4; i++){
                    allExist = allExist && wifiNode.exists[i];
                }
                if(allExist){
                    xSemaphoreGive(s4436572_SemaphoreBLEScanStop);
                    vTaskDelay(100);
                    for(uint8_t i=0; i<4; i++){
                        wifiNode.exists[i] = false;
                    }
                    console_log(DEBUG, "Queue receive sending mobile data\r\n");
                    BSP_MAGNETO_GetXYZ(axes);
                    LSM6DSL_GyroReadStep(&steps);
                    angle = round(180*atan2(axes[1], axes[0])/3.14159265);
                    console_log(LOG, "X: %d, Y: %d, Z: %d, Angle: %d\r\n", axes[0], axes[1], axes[2], angle);
                    wifiNode.heading = angle;
                    console_log(LOG, "Steps: %d\r\n", steps);
                    wifiNode.step = steps - old_steps;
                    old_steps = steps;
                    sprintf(serialData, "{\"rssi\": [%d, %d, %d, %d], \"uss\":[%d, %d, %d, %d], \"step\": %d, \"angle\": %d}", 
                        wifiNode.nodes[0].rssi, wifiNode.nodes[1].rssi, wifiNode.nodes[2].rssi, wifiNode.nodes[3].rssi, 
                        wifiNode.nodes[0].major, wifiNode.nodes[1].major, wifiNode.nodes[2].major, wifiNode.nodes[3].major,
                        wifiNode.step, wifiNode.heading);
                    strcpy(wPacket.data, serialData);
                    wPacket.length = strlen(wPacket.data);
                    xQueueSend(s4436572_QueueWifiSend, &wPacket, 10);

                    if(xSemaphoreTake(s4436572_SemaphoreWifiSent, portMAX_DELAY)){
                        xSemaphoreGive(s4436572_SemaphoreBLEScanStart);
                    }
                }
            }
		}
		vTaskDelay(100);
	}
}
