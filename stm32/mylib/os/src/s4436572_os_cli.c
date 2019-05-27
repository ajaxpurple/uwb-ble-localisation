/**
 ******************************************************************************
 * @file    mylib/s4436572_os_cli.c
 * @author  Rohan Malik � 44365721
 * @date    17032019
 * @brief   FreeRTOS CLI Task
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_cli_init() - Start CLI task
 * s4436572_os_cli_deinit() - Stop CLI task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "string.h"
#include "s4436572_hal_uart.h"
#include "s4436572_os_log.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "s4436572_os_cli.h"
#include <stdlib.h>
#include "s4436572_os_led.h"
#include "s4436572_os_wifi.h"
#include "task.h"

//Task stack size and priority
#define EXTRA_PRIORITY 		2 	//Priority for the task in addition to the idle priority
#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 3)
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY

#define MAX_ARGS 10

//Input size
#define INPUT_SIZE 100

extern void s4436572_TaskCLI(void);

//CLI task handle
TaskHandle_t s4436572_TaskHandleCLI;
QueueHandle_t s4436572_QueueAddCommand;
QueueHandle_t s4436572_QueueLineInput;
Command* commands = NULL;

//Declare variables
//char inputString[INPUT_SIZE];
//int inputIndex = 0;
int numCommands = 0;
LineInput input;

/**
  * @brief  Initialise CLI.
  * @param  None
  * @retval None
  */
extern void s4436572_os_cli_init(){
		s4436572_QueueAddCommand = xQueueCreate(10, sizeof(Command));
		s4436572_QueueLineInput = xQueueCreate(10, sizeof(LineInput));
    xTaskCreate( (TaskFunction_t) &s4436572_TaskCLI, (const char *) "cli", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleCLI);
}

/**
  * @brief  Deinitialise CLI.
  * @param  None
  * @retval None
  */
extern void s4436572_os_cli_deinit(void){
    vTaskDelete(s4436572_TaskHandleCLI);
}

void add_line_input(char* line){
	strcpy(input.line, line);
	xQueueSendFromISR(s4436572_QueueLineInput, &input, NULL);
}

void execute_line(char* line){
	int argc = 0;
	char* argv[MAX_ARGS];  
	char* rest = line;
	char* idx;
	while ((idx = (char*) strtok_r(rest, " ", &rest)) && argc < MAX_ARGS - 1){
			argv[argc] = idx;
			argc++; 
	}
	argv[argc] = 0;

	int returnValue = -1;

	for(int i=0; i<numCommands; i++){
		if(strcmp(commands[i].name, argv[0]) == 0){
			returnValue = (*(commands[i].func))(argc, argv);
		}
	}
	if(returnValue == -1){
		console_log(ERR, "Command not found\r\n");
	}else if(returnValue > 0){
		console_log(ERR, "\r");
		console_log(ERR, "Command error: %d errors found\r\n", returnValue);
	}
}

/**
  * @brief  CLI Receive Task.
  * @param  None
  * @retval None
  */
extern void s4436572_TaskCLI(void) {

	Command command;
	WifiPacket packet;
	/* Initialise pointer to CLI output buffer. */

	hal_uart_register_rx(&add_line_input);

	for (;;) {
		if(xQueueReceive(s4436572_QueueLineInput, &input, 0)){
			//execute_line(input.line);
			strcpy(packet.data, input.line); 
    	packet.length = strlen(packet.data);
			xQueueSend(s4436572_QueueWifiSend, &packet, 0);
		}

		if(xQueueReceive(s4436572_QueueAddCommand, &command, 0)){
			numCommands++;
			commands = realloc(commands, numCommands*sizeof(Command));
			commands[numCommands - 1] = command;
		}
		vTaskDelay(10);
	}
}
