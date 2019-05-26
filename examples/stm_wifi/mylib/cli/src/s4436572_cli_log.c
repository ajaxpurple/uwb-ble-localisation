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
#include "s4436572_cli_log.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
int log_argtable(int argc, char *argv[]);
Command command;

/**
  * @brief  Initialise CLI led.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_log_init(void) {
    command = (Command){.name="log", .func=&log_argtable};
    xQueueSendToBack(s4436572_QueueAddCommand, &command, TICK_COUNT);        
}

/**
  * @brief  Callback for led command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int log_argtable(int argc, char *argv[]){
	    /* global arg_xxx structs */
    struct arg_lit* help;
    struct arg_str* filter;
    struct arg_end* end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "display this help and exit"),
        filter   = arg_str0("f", "filter", "{log, debug, error, all}", "Filter log messages"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);

    if(filter->count > 0){
        int returnValue = 0;
        if(strcmp(filter->sval[0], "log") == 0){
            xSemaphoreGive(s4436572_SemaphoreFilterLog);
            console_log(DEBUG, "Semaphore give filter log\r\n");
        }else if(strcmp(filter->sval[0], "debug") == 0){
            xSemaphoreGive(s4436572_SemaphoreFilterDebug);
            console_log(DEBUG, "Semaphore give filter debug\r\n");
        }else if(strcmp(filter->sval[0], "error") == 0){
            xSemaphoreGive(s4436572_SemaphoreFilterError);
            console_log(DEBUG, "Semaphore give filter error\r\n");
        }else if(strcmp(filter->sval[0], "all") == 0){
            xSemaphoreGive(s4436572_SemaphoreFilterAll);
            console_log(DEBUG, "Semaphore give filter all\r\n");
        }else{
            returnValue = 1;
        }
        return returnValue;
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Filter log messages\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
}