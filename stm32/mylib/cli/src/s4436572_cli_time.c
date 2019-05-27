/**
 ******************************************************************************
 * @file    mylib/s4436572_cli_time.c
 * @author  Rohan Malik � 44365721
 * @date    17032019
 * @brief   System timer RTOS CLI functions
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_cli_time_init() � initialise time CLI
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_cli_time.h"
#include "s4436572_os_time.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
int time_argtable(int argc, char *argv[]);

Command command;

/**
  * @brief  Initialise CLI led.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_time_init(void) {
    command = (Command){.name="time", .func=&time_argtable};
    xQueueSendToBack(s4436572_QueueAddCommand, &command, TICK_COUNT);        
}

/**
  * @brief  Callback for led command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int time_argtable(int argc, char *argv[]){
	/* global arg_xxx structs */
	struct arg_lit *help, *format;
	struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "display this help and exit"),
        format   = arg_lit0("f", "format", "Show system time in hours, minutes and seconds"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);
  	
	if(help->count > 0){
		console_log(NONE, "Usage: %s ", argv[0]);
		arg_print_syntax(stdout, argtable, "\r\n");
		console_log(NONE,"Print system time in seconds\r\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
		return 0;
	}else if (nerrors > 0){
		arg_print_errors(stdout, end, argv[0]);
		console_log(ERR, "\r");
		console_log(ERR, "Try help for more information.\r\n");
		return nerrors;
	}else if(format->count){
		console_log(DEBUG, "Semaphore give hms\r\n");
		xSemaphoreGive(s4436572_SemaphoreGetHMS);
		return 0;
	}else{
		console_log(DEBUG, "Semaphore give seconds\r\n");
		xSemaphoreGive(s4436572_SemaphoreGetSeconds);
		return 0;
	}
}