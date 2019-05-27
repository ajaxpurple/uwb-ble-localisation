/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_ble.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   Bluetooth HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_ble_init() - Setup Bluetooth
 * s4436572_hal_ble_deinit() - Deinitialise Bluetooth
 * s4436572_hal_ble_write(message) - Write to Bluetooth
 * s4436572_hal_ble_read(message, length) - Read from Bluetooth
 ******************************************************************************
 */
#ifndef S4436572_HAL_BLE_H
#define S4436572_HAL_BLE_H
#include "bluenrg_def.h"

typedef struct{
  uint8_t address[6];
  uint8_t length;
  uint8_t data[50];
  int8_t rssi;
}Advertisement;

typedef struct{
  uint8_t uuid[16];
  uint16_t major;
  uint16_t minor;
  int8_t rssiAt1m;
  int8_t rssi;
}IBeaconData;
int extract_ibeacon_data(Advertisement advertisement, IBeaconData* beaconData);
void s4436572_hal_ble_init();
void s4436572_hal_ble_set_uuid(uint8_t* uuid);
tBleStatus s4436572_hal_ble_advertise(uint16_t major, uint16_t minor);
void s4436572_hal_ble_scan_start(void (*callback)(Advertisement adv));
void s4436572_hal_ble_scan_stop(void);
void s4436572_hal_ble_advertise_stop(void);
void s4436572_hal_ble_advertise_start(void);
void s4436572_hal_ble_check();


#endif