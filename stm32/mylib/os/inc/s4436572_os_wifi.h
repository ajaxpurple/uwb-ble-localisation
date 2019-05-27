/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_wifi.h
 * @author  Rohan Malik - 44365721
 * @date    11042019
 * @brief   Wifi RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_wifi_init() - initialise wifi
 * s4436572_os_wifi_deinit() - de-initialise wifi
 ******************************************************************************
 */

#ifndef S4436572_OS_WIFI_H
#define S4436572_OS_WIFI_H

#include "FreeRTOS.h"
#include "semphr.h"

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

typedef struct{
    char data[100];
    uint16_t length;
}WifiPacket;

typedef struct{
    uint8_t address[4];
    uint16_t port;
}Connection;

typedef struct{
    char SSID[20];
    char password[20];
}NetworkConfig;

extern QueueHandle_t s4436572_QueueWifiNetworkConfig;
extern QueueHandle_t s4436572_QueueWifiConnectionConfig;
extern QueueHandle_t s4436572_QueueWifiSend; //Wifi send queue
extern QueueHandle_t s4436572_QueueWifiReceive; //Wifi receive queue
extern SemaphoreHandle_t s4436572_SemaphoreWifiOff;
extern SemaphoreHandle_t s4436572_SemaphoreWifiScan;
extern SemaphoreHandle_t s4436572_SemaphoreWifiJoin;
extern SemaphoreHandle_t s4436572_SemaphoreWifiConnect;
extern SemaphoreHandle_t s4436572_SemaphoreWifiSuccess;
extern SemaphoreHandle_t s4436572_SemaphoreWifiSent;
extern SemaphoreHandle_t s4436572_SemaphoreWifiReceiveOn;
extern SemaphoreHandle_t s4436572_SemaphoreWifiReceiveOff;
extern SemaphoreHandle_t s4436572_SemaphoreWifiAccelToggle;

void s4436572_os_wifi_init(void); //Initialise wifi
void s4436572_os_wifi_deinit(void); //De-initialise wifi
#endif
