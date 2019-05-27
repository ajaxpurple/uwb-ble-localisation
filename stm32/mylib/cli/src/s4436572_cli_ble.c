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
#include "s4436572_cli_ble.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"
#include "s4436572_os_ble.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
int ble_argtable(int argc, char *argv[]);
Command command;



/**
  * @brief  Initialise CLI led.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_ble_init(void) {
    command = (Command){.name="ble", .func=&ble_argtable};
    xQueueSendToBack(s4436572_QueueAddCommand, &command, TICK_COUNT);  
    //memset(filter.address, 0, 20);      
}

/**
  * @brief  Callback for led command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int ble_argtable(int argc, char *argv[]){
	    /* global arg_xxx structs */
    struct arg_lit *argHelp, *argAccelAd;
    struct arg_str *argFilter, *argScan, *argFilterAddr, *argAd;
    struct arg_int *argMajor, *argMinor;
    struct arg_end *end;
    BLEPacket packet;
    BLEFilter filter; 
    int retval = 0;

    void *argtable[] = {
        argHelp     = arg_lit0("h", "help", "display this help and exit"),
        argFilterAddr = arg_str0("d", "address", "<xx:xx:xx:xx:xx:xx>", "Set filter address"),
        argFilter     = arg_str0("f", "filter", "{on, off}", "Filter on or off"),
        argScan     = arg_str0("s", "scan", "{on, off}", "Scan on or off"),
        argMajor    = arg_int0("M", "major","<n>", "Set major value"),
        argMinor    = arg_int0("m", "minor","<n>", "Set minor value"),
        argAd       = arg_str0("a", "advertise","{on, off}" , "Advertise"),
        argAccelAd  = arg_lit0("x", "accelxy", "Toggle advertisement of accelerometer values"),
        end         = arg_end(10)
    };

    int nerrors = arg_parse(argc,argv,argtable);

    if(argMajor->count > 0 && argMinor->count > 0){
        packet.major = argMajor->ival[0];
        packet.minor = argMinor->ival[0];   
        xQueueSend(s4436572_QueueBLEAdvertise, &packet,10);
    }
    if(argAccelAd->count > 0){
        xSemaphoreGive(s4436572_SemaphoreBLEAccelToggle);
        console_log(DEBUG, "Semaphore give accel toggle\r\n");
    }else if(argScan->count > 0){
        if(strcmp(argScan->sval[0], "on") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEScanStart);
            console_log(DEBUG, "Semaphore give ble scan start\r\n");
        }else if(strcmp(argScan->sval[0], "off") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEScanStop);
            console_log(DEBUG, "Semaphore give ble scan stop\r\n");
        }else{
            retval = 1;
        }
    }else if(argFilterAddr->count > 0){
        for(uint8_t i=0; i<17; i++){
            filter.address[i] = argFilterAddr->sval[0][i];
        }
        filter.address[17] = '\0';
        xQueueSend(s4436572_QueueBLEFilterAddress, &filter, 10);
        console_log(DEBUG, "Queue overwrite filter address\r\n");
    }else if(argAd->count > 0){
        if(strcmp(argAd->sval[0], "on") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEAdvertiseStart);
            console_log(DEBUG, "Semaphore give ble advertise start\r\n");
        }else if(strcmp(argAd->sval[0], "off") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEAdvertiseStop);
            console_log(DEBUG, "Semaphore give ble advertise stop\r\n");
        }else{
            retval = 1;
        }
    }else if(argFilter->count > 0){
        if(strcmp(argFilter->sval[0], "on") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEFilterOn);
            console_log(DEBUG, "Semaphore give ble filter on\r\n");
        }else if(strcmp(argFilter->sval[0], "off") == 0){
            xSemaphoreGive(s4436572_SemaphoreBLEFilterOff);
            console_log(DEBUG, "Semaphore give ble filter off\r\n");
        }else{
            retval = 1;
        }
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Bluetooth scanning and advertising\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
    return retval;
}