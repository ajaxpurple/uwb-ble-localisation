#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32l475xx.h"
#include "stm32l4xx_hal.h"
#include "s4436572_hal_board.h"
#include "wifi.h"
#include "s4436572_hal_wifi.h"

#define TIMEOUT 200
#define SOCKET 0

char wifiMessage[100];


WifiInfo info;
WIFI_APs_t aps;

void wifi_led_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = WIFI_BLE_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(WIFI_BLE_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(WIFI_BLE_GPIO_PORT, WIFI_BLE_GPIO_PIN, GPIO_PIN_SET);
}


int s4436572_hal_wifi_init(){
    int status = WIFI_Init();
    WIFI_GetMAC_Address(info.macAddress);
    wifi_led_init();
    return status;
}

ScanResult s4436572_hal_wifi_scan(){
    SPI_WIFI_ReInit();
    WIFI_ListAccessPoints(&aps, WIFI_MAX_APS);
    ScanResult result;
    for(uint8_t i=0; i<aps.count; i++){
        strcpy(result.ap[i].SSID, aps.ap[i].SSID);
        result.ap[i].RSSI = aps.ap[i].RSSI;
    }
    result.count = aps.count;
    return result;
}

int s4436572_hal_wifi_join(char* ssid, char* password){
    WIFI_Status_t status = 0;
    SPI_WIFI_ReInit();
    status = WIFI_Connect(ssid, password, WIFI_ECN_WPA2_PSK);
    WIFI_GetIP_Address(info.clientIP);
    return status;
}
void s4436572_hal_wifi_disconnect(){
    SPI_WIFI_ReInit();
    WIFI_Disconnect();
}

int s4436572_hal_wifi_open(uint8_t socket, char* connectionName, uint8_t* serverIP, uint16_t port){
    int status;
    SPI_WIFI_ReInit();
    status = WIFI_OpenClientConnection(socket, WIFI_UDP_PROTOCOL, connectionName, serverIP, port, 0);
    return status;
}

int s4436572_hal_wifi_close(uint8_t socket){
    int status = 1;
    SPI_WIFI_ReInit();
    status = WIFI_CloseClientConnection(socket);
    return status;
}

uint16_t s4436572_hal_wifi_read(uint8_t socket, uint8_t* rxData, uint16_t rxLength){
    uint16_t recvLength;
    SPI_WIFI_ReInit();
    WIFI_ReceiveData(socket, rxData, rxLength, &recvLength, TIMEOUT);
    return recvLength;
}

uint16_t s4436572_hal_wifi_ping(uint8_t* ipAddr){
    SPI_WIFI_ReInit();
    return WIFI_Ping(ipAddr, 5, 50);
}

uint16_t s4436572_hal_wifi_write(uint8_t socket, uint8_t* txData, uint16_t txLength){
    uint8_t length = 0;
    SPI_WIFI_ReInit();
    WIFI_SendData(socket, txData, txLength, NULL, 50);
    WIFI_ReceiveData(socket, &length, 1, NULL, 50);
    return length;
}

WifiInfo s4436572_hal_wifi_info(){
    return info;
}