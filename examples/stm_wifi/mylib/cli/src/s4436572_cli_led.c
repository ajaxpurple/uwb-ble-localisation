/**
 ******************************************************************************
 * @file    mylib/s4436572_cli_led.c
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Led RTOS CLI functions
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_cli_led_init() � initialise led CLI
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_cli_led.h"
#include "s4436572_os_led.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
int led_argtable(int argc, char *argv[]);

Command command;
LedCommand ledCommand;

/**
  * @brief  Initialise CLI led.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_led_init(void) {
    command = (Command){.name="led", .func=&led_argtable};
    xQueueSendToBack(s4436572_QueueAddCommand, &command, TICK_COUNT);        
}

/**
  * @brief  Callback for led command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int led_argtable(int argc, char *argv[]){

    /* global arg_xxx structs */
    struct arg_lit *help, *blink;
    struct arg_int *on, *off, *toggle;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "display this help and exit"),
        on    	= arg_int1("o", "on", "<n>", "turn on LED"),
        off    	= arg_int1("f", "off", "<n>","turn off LED"),
        toggle  = arg_int1("t", "toggle", "<n>", "toggle LED"),
        blink 	= arg_lit0("b", "blink_log", "Blink LED1 and spam messages"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);

    if(on-> count + toggle-> count + off-> count> 0){
        if(on->count > 0){
            ledCommand.action = ON;
            ledCommand.led = *(on->ival);
        }else if(off->count > 0){
            ledCommand.action = OFF;        
            ledCommand.led = *(off->ival);
        }else if(toggle->count > 0){
            ledCommand.action = TOGGLE;             
            ledCommand.led = *(toggle->ival);
        }
        console_log(DEBUG, "Queue add LED=%d\r\n", ledCommand.action);
        xQueueSendToBack(s4436572_QueueLedCommand, &ledCommand, TICK_COUNT);
        console_log(LOG, "Set LED state to %d\r\n", ledCommand.action);
        return 0;
    }else if(blink->count > 0){
        xSemaphoreGive(s4436572_SemaphoreLedToggleBlink);
        console_log(LOG, "Toggled blink with messages task\r\n");
        return 0;
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Control LED\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
}