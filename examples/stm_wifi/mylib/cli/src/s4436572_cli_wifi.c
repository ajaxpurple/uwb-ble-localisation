/**
 ******************************************************************************
 * @file    mylib/s4436572_cli_log.c
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   Logging RTOS CLI
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_cli_log_init() � initialise log CLI
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_cli_wifi.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"
#include "s4436572_os_wifi.h"
#include "s4436572_hal_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
int wifi_argtable(int argc, char *argv[]);

Command command;
WifiPacket packet;
Connection connection;
NetworkConfig config;

uint8_t parse_hex_digit2(uint8_t letter){
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
  * @brief  Initialise CLI wifi.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_wifi_init(void) {
    command = (Command){.name="wifi", .func=&wifi_argtable};
    xQueueSendToBack(s4436572_QueueAddCommand, &command, TICK_COUNT);      
}

/**
  * @brief  Callback for wifi command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int wifi_argtable(int argc, char *argv[]){
	    /* global arg_xxx structs */
    struct arg_lit *argHelp, *argScan, *argJoin, *argConnect, *argToggle;
    struct arg_str *argSSID, *argPass, *argSend, *argReceive;
    struct arg_int *argPort, *argAddr;
    struct arg_end *end;
    int retval = 0;

    void *argtable[] = {
        argHelp     = arg_lit0("h", "help", "display this help and exit"),
        
        argScan     = arg_lit0("s", "scan", "Scan for networks"),
        argSSID     = arg_str0("i", "ssid", "<str>", "Network SSID"),
        argPass     = arg_str0("p", "password", "<str>", "Network password"),
        argJoin     = arg_lit0("j", "join", "Join network"),
        
        argAddr     = arg_intn("d", "address", "<int>", 0, 4, "Set connection address"),
        argPort     = arg_int0("n", "port", "<int>", "Set port"),
        argConnect  = arg_lit0("c", "connect", "Connect to server"),
        argSend     = arg_str0("w", "send", "<str>", "Send a message"),
        argReceive  = arg_str0("r", "receive", "{on, off}", "Receive on or off"),
        argToggle  = arg_lit0("x", "toggle", "Test wifi"),
        end         = arg_end(10)
    };

    int nerrors = arg_parse(argc,argv,argtable);

    if(argScan->count > 0){
        xSemaphoreGive(s4436572_SemaphoreWifiScan);
        console_log(DEBUG, "Semaphore give wifi scan\r\n");
    }else if(argSSID->count > 0 && argPass->count > 0){
        strcpy(config.SSID, argSSID->sval[0]);
        strcpy(config.password, argPass->sval[0]);
        xQueueSend(s4436572_QueueWifiNetworkConfig, &config, 10);
    }else if(argJoin->count > 0){
        xSemaphoreGive(s4436572_SemaphoreWifiJoin);
        console_log(DEBUG, "Semaphore give wifi join\r\n");
    }else if(argAddr->count > 0 && argPort->count > 0){
        for(uint8_t i=0; i<4; i++){
            connection.address[i] = argAddr->ival[i];
        }
        connection.port = argPort->ival[0];
        xQueueSend(s4436572_QueueWifiConnectionConfig, &connection, 10);
    }else if(argConnect->count > 0){
        xSemaphoreGive(s4436572_SemaphoreWifiConnect);
        console_log(DEBUG, "Semaphore give wifi connect\r\n");
    }else if(argSend->count > 0){
        strcpy(packet.data, argSend->sval[0]); 
        packet.length = strlen(packet.data);
        xQueueSend(s4436572_QueueWifiSend, &packet, 10);
    }else if(argReceive->count > 0){
        if(strcmp(argReceive->sval[0], "on") == 0){
            xSemaphoreGive(s4436572_SemaphoreWifiReceiveOn);
            console_log(DEBUG, "Semaphore give wifi receive on\r\n");
        }else if(strcmp(argReceive->sval[0], "off") == 0){
            xSemaphoreGive(s4436572_SemaphoreWifiReceiveOff);
            console_log(DEBUG, "Semaphore give wifi receive off\r\n");
        }else{
            retval = 1;
        }
    }else if(argToggle->count > 0){
        xSemaphoreGive(s4436572_SemaphoreWifiAccelToggle);
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Wifi\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
    return retval;
}