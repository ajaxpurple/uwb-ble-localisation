/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_cli.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   CLI RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_cli_init() - Start CLI task
 * s4436572_os_cli_deinit() - Stop CLI task
 ******************************************************************************
 */

#ifndef S4436572_OS_CLI_H
#define S4436572_OS_CLI_H
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t s4436572_QueueAddCommand;
extern QueueHandle_t s4436572_QueueLineInput;
typedef struct{
	char* name;
	int (*func)(int, char**);
}Command;

typedef struct{
	char line[100];
}LineInput;

void s4436572_os_cli_init();
void s4436572_os_cli_deinit(void);

#endif
