/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_wifi.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   WiFi HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_wifi_init() - Setup WiFi
 * s4436572_hal_wifi_send_page() - Send a page
 * s4436572_hal_wifi_disconnect() - Disconnect from wifi
 * s4436572_hal_wifi_read(message, length) - Read from UART
 ******************************************************************************
 */
#ifndef S4436572_HAL_WIFI_H
#define S4436572_HAL_WIFI_H
#include "stm32l4xx.h"

typedef struct{
    uint8_t macAddress[6];
    uint8_t clientIP[4];
}WifiInfo;

typedef struct{
    char SSID[100];
    int8_t RSSI;
}AccessPoint;

typedef struct{
    AccessPoint ap[20];
    uint8_t count;
}ScanResult;

int s4436572_hal_wifi_init();
ScanResult s4436572_hal_wifi_scan();
void s4436572_hal_wifi_disconnect();
int s4436572_hal_wifi_join(char* ssid, char* password);
uint16_t s4436572_hal_wifi_ping(uint8_t* ipAddr);
int s4436572_hal_wifi_open(uint8_t socket, char* connectionName, uint8_t* serverIP, uint16_t port);
int s4436572_hal_wifi_close(uint8_t socket);

uint16_t s4436572_hal_wifi_read(uint8_t socket, uint8_t* rxData, uint16_t rxLength);
uint16_t s4436572_hal_wifi_write(uint8_t socket, uint8_t* txData, uint16_t txLength);
WifiInfo s4436572_hal_wifi_info();

#endif