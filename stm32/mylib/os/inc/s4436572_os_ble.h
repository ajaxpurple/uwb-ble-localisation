/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_ble.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Bluetooth RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_ble_init() - initialise bluetooth
 * s4436572_os_ble_deinit() - de-initialise bluetooth
 ******************************************************************************
 */

#ifndef S4436572_OS_BLE_H
#define S4436572_OS_BLE_H

#include "semphr.h"
#include "config.h"


typedef struct{
    uint16_t major;
    uint16_t minor;
}BLEPacket;

typedef struct{
    char address[20];
}BLEFilter;

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

extern QueueHandle_t s4436572_QueueBLEAdvertise; //Bluetooth queue
extern QueueHandle_t s4436572_QueueBLEReceive; //Bluetooth queue
extern QueueHandle_t s4436572_QueueBLEBeacon; //Bluetooth queue
extern QueueHandle_t s4436572_QueueBLEFilterAddress; 
extern SemaphoreHandle_t s4436572_SemaphoreBLEFilterOn;
extern SemaphoreHandle_t s4436572_SemaphoreBLEFilterOff;
extern SemaphoreHandle_t s4436572_SemaphoreBLEScanStart;
extern SemaphoreHandle_t s4436572_SemaphoreBLEScanStop;
extern SemaphoreHandle_t s4436572_SemaphoreBLEAccelToggle;
extern SemaphoreHandle_t s4436572_SemaphoreBLEAdvertiseStop;
extern SemaphoreHandle_t s4436572_SemaphoreBLEAdvertiseStart;

void s4436572_os_ble_init(void); //Initialise bluetooth
void s4436572_os_ble_deinit(void); //De-initialise bluetooth
#endif
