/**
 ******************************************************************************
 * @file    mylib/s4436572_os_log.c
 * @author  Rohan Malik � 44365721
 * @date    17032019
 * @brief   RTOS Task safe log wrapper
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_print(const char* format, ...) - Task safe log
 * s4436572_os_log_init() - initialise system log
 * s4436572_os_log_deinit() - de-initialise system log
 * int s4436572_os_puts(char* string) - Task safe puts
 * int s4436572_os_log(MessageType msg_type, const char* format, ...) - Task safe log
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4436572_os_log.h"
#include "s4436572_hal_printf.h"
#include "s4436572_hal_wifi.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define LOG_QUEUE_SIZE 20 //Log queue size
#define TICK_COUNT 10 		//Default waiting period for queues and semaphores

//Task stack size and priority
#define EXTRA_PRIORITY 		2
#define TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 5 )
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Mylog task handle, queue, semaphore and message struct
TaskHandle_t s4436572_TaskHandleLog;
QueueHandle_t s4436572_QueueLog = NULL;
SemaphoreHandle_t s4436572_SemaphoreLog = NULL;
SemaphoreHandle_t s4436572_SemaphoreFilterLog;
SemaphoreHandle_t s4436572_SemaphoreFilterDebug;
SemaphoreHandle_t s4436572_SemaphoreFilterError;
SemaphoreHandle_t s4436572_SemaphoreFilterAll;
Message message;
Message messagePuts;

void s4436572_TaskLog(void);

typedef struct{
	bool allow_log;
	bool allow_debug;
	bool allow_error;
}MessageFilter;

/**
  * @brief Mylog initialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_log_init(void){
	s4436572_hal_printf_init();
	
	//Create the log queue and semaphore
	s4436572_QueueLog = xQueueCreate(LOG_QUEUE_SIZE, sizeof(Message));
	s4436572_SemaphoreLog = xSemaphoreCreateBinary();
	s4436572_SemaphoreFilterAll = xSemaphoreCreateBinary();
	s4436572_SemaphoreFilterDebug = xSemaphoreCreateBinary();
	s4436572_SemaphoreFilterError = xSemaphoreCreateBinary();
	s4436572_SemaphoreFilterLog = xSemaphoreCreateBinary();

	//Give the semaphore initially
	xSemaphoreGive(s4436572_SemaphoreLog);

	//Initialise message counter
	message.Sequence_Number = 0;

	//Start log task
	xTaskCreate( (TaskFunction_t) &s4436572_TaskLog, (const char *) "log",
			TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleLog);

}

/**
  * @brief Mylog deinitialisation
  * @param  None
  * @retval None
*/
extern void s4436572_os_log_deinit(void){

	//Delete the task
	vTaskDelete(s4436572_TaskHandleLog);

	//Delete the log queue and semaphore
	vQueueDelete(s4436572_QueueLog);
	vSemaphoreDelete(s4436572_SemaphoreLog);
	vSemaphoreDelete(s4436572_SemaphoreFilterAll);
	vSemaphoreDelete(s4436572_SemaphoreFilterLog);
	vSemaphoreDelete(s4436572_SemaphoreFilterDebug);
	vSemaphoreDelete(s4436572_SemaphoreFilterError);
	s4436572_hal_printf_deinit();
}

/**
  * @brief mylog function - Task safe wrapper for log
  * @param  format - The string template, args (...) - Remaining arguments
  * @retval None
*/
extern int s4436572_os_log(MessageType msg_type, const char* format, ...){
	int length = -1;
	//Take the semaphore to ensure vsprintf is task safe
	if(xSemaphoreTake(s4436572_SemaphoreLog, 1) == pdTRUE){

		//Store message to be printed in a string
		va_list arguments;
		va_start(arguments, format);
		vsprintf(message.Payload, format, arguments);
		va_end(arguments);

		length = strlen(message.Payload);
		message.MsgType = msg_type;

		//Add string message to queue
		if(s4436572_QueueLog != NULL){
			xQueueSendToBack(s4436572_QueueLog, (void*) &message, portMAX_DELAY);
		}

		message.Sequence_Number++; //Increment message counter
		xSemaphoreGive(s4436572_SemaphoreLog); //Give the semaphore back
	}
	return length;
}

/**
  * @brief mylog function - Task safe wrapper for puts
  * @param  format - The string template, args (...) - Remaining arguments
  * @retval None
*/
int s4436572_os_puts(char* string){
	int length = -1;
	
	strcpy(messagePuts.Payload, string);
	length = strlen(messagePuts.Payload);
	messagePuts.MsgType = NONE;

	//Add string message to queue
	if(s4436572_QueueLog != NULL){
		xQueueSendToBack(s4436572_QueueLog, (void*) &messagePuts, 0);
	}

	messagePuts.Sequence_Number = 0;
	return length;
}

int _write(int file, char* ptr, int len){
	s4436572_os_puts(ptr);
	return len;
}


int debug_fputc(int chr, FILE* fp){
	char charString[3];
	charString[0] = chr;
	charString[1] = '\r';
	charString[2] = '\0';
	s4436572_os_puts(charString);
	return chr;
}

int debug_fprintf(FILE *stream, const char *format, ...){
	int messageType;
	if(fileno(stream) == STDOUT_FILENO){
			messageType = NONE;
	}else{
			messageType = ERR;
	}

	int length = -1;

	//Take the semaphore to ensure vslog is task safe
	if(xSemaphoreTake(s4436572_SemaphoreLog, 0) == pdTRUE){

		//Store message to be printed in a string
		va_list arguments;
		va_start(arguments, format);
		vsprintf(message.Payload, format, arguments);
		va_end(arguments);

		length = strlen(message.Payload);
		message.MsgType = messageType;

		//Add string message to queue
		if(s4436572_QueueLog != NULL){
			xQueueSendToBack(s4436572_QueueLog, (void*) &message, 0);
		}

		message.Sequence_Number++; //Increment message counter
		xSemaphoreGive(s4436572_SemaphoreLog); //Give the semaphore back
	}
	return length;
}

/**
  * @brief Mylog task - task that prints all the messages from the queue
  * @param  None
  * @retval None
*/
extern void s4436572_TaskLog(void){

	//Message received from queue
	Message recvMessage;
	char* prefixes[] = {"LOG", "ERROR", "DEBUG"};
	char* prefix = NULL;
	MessageFilter filter = (MessageFilter){.allow_error=true, .allow_log = true, .allow_debug=true};
	bool allow = false;
	bool showPrefix = true;
	uint8_t lastIndex;
	for(;;){
		//Receive messages to be printed from queue
		if(xQueueReceive(s4436572_QueueLog, &recvMessage, 10)){
			allow = false;
			switch(recvMessage.MsgType){
				case LOG: 
					prefix = prefixes[0]; 
					allow = filter.allow_log; 
					break;
				case ERR: 
					prefix = prefixes[1]; 
					allow = filter.allow_error; 
					break;
				case DEBUG: 
					prefix = prefixes[2]; 
					allow = filter.allow_debug; 
					break;
				case NONE: 
					allow = true; 
					break;
			}
			if(allow || recvMessage.MsgType == NONE){
				
				if(recvMessage.MsgType != NONE){
					set_color(recvMessage.MsgType);
					
					if(showPrefix){
						vTaskDelay(2);
						debug_printf("%s: ", prefix);
						showPrefix = false;
					}

				}
				vTaskDelay(2);
				debug_printf("%s", recvMessage.Payload);
				vTaskDelay(2);
				lastIndex = strlen(recvMessage.Payload);
				if(recvMessage.Payload[lastIndex - 1] == '\r' || recvMessage.Payload[lastIndex - 1] == '\n'){
					showPrefix = true;
				}
				if(recvMessage.MsgType != NONE){
					vTaskDelay(3);
					reset_color();
				}

			}
		}

		if(xSemaphoreTake(s4436572_SemaphoreFilterAll, 0) == pdTRUE){
			filter = (MessageFilter){.allow_error=true, .allow_log = true, .allow_debug=true};
			console_log(NONE, "Show all messages\r\n");
		}else if(xSemaphoreTake(s4436572_SemaphoreFilterLog, 0) == pdTRUE){
			filter = (MessageFilter){.allow_error=false, .allow_log = true, .allow_debug=false};
			console_log(NONE, "Show log messages only\r\n");
		}else if(xSemaphoreTake(s4436572_SemaphoreFilterDebug, 0) == pdTRUE){
			filter = (MessageFilter){.allow_error=false, .allow_log = false, .allow_debug=true};
			console_log(NONE, "Show debug messages only\r\n");
		}else if(xSemaphoreTake(s4436572_SemaphoreFilterError, 0) == pdTRUE){
			filter = (MessageFilter){.allow_error=true, .allow_log = false, .allow_debug=false};
			console_log(NONE, "Show error messages only\r\n");
		}
		vTaskDelay(10);
	}
}
