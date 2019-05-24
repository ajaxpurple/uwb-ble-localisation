#ifndef OS_BLE_H
#define OS_BLE_H

#include "FreeRTOS.h"
#include "semphr.h"
void ble_setup(void);
void ble_setid(uint8_t id);
void bleTask(void);
void scan_start(void);

extern SemaphoreHandle_t bleBroadcastStart;
extern SemaphoreHandle_t bleBroadcastStop;
extern SemaphoreHandle_t bleScanStart;
extern SemaphoreHandle_t bleScanStop;
extern QueueHandle_t bleSetId;
extern QueueHandle_t bleScanResult;
#endif