/**
 ******************************************************************************
 * @file    mylib/s4436572_os_fatfs.c
 * @author  Rohan Malik - 44365721
 * @date    15032019
 * @brief   Fat Filesystem RTOS
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_fatfs_init() - initialise fatfs
 * s4436572_os_fatfs_deinit() - de-initialise fatfs
 * s4436572_TaskFatfs() - fatfs task
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_os_fatfs.h"
#include "s4436572_hal_fatfs.h"
#include "s4436572_os_log.h"
#include "task.h"
#include "semphr.h"
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 60
#define TICK_COUNT 10 //Waiting time for semaphores and queues
#define FATFS_QUEUE_SIZE 5 //Queue size for fatfs

#define TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE * 30)
#define EXTRA_PRIORITY 		3
#define TASK_PRIORITY 		tskIDLE_PRIORITY + EXTRA_PRIORITY


//Task, queues and semaphores for servo
TaskHandle_t s4436572_TaskHandleFatfs;
QueueHandle_t s4436572_QueueFatfsCommand;


void s4436572_TaskFatfs();

/**
  * @brief  Initialise fatfs.
  * @param  None
  * @retval None
*/
extern void s4436572_os_fatfs_init(void) {

	//Initialise queues and semaphores
	s4436572_QueueFatfsCommand = xQueueCreate(FATFS_QUEUE_SIZE, sizeof(FatfsCommand));
	
	//Create task
	xTaskCreate( (TaskFunction_t) &s4436572_TaskFatfs, (const char *) "fatfs", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &s4436572_TaskHandleFatfs);
}


/**
  * @brief  De-initialise fatfs.
  * @param  None
  * @retval None
*/
extern void s4436572_os_fatfs_deinit(void) {

	//Delete task, queues and semaphore
	vTaskDelete(s4436572_TaskHandleFatfs);
	vQueueDelete(s4436572_QueueFatfsCommand);

	//portDISABLE_INTERRUPTS();
	//s4436572_hal_fatfs_deinit(); //Deinitialise fatfs
	//portENABLE_INTERRUPTS();

}

/**
  * @brief fatfs RTOS Task function
  * @param  None
  * @retval None
*/
extern void s4436572_TaskFatfs(void){
	s4436572_hal_fatfs_init();
	
	FIL file[2];
	bool freeFileHandle;
	bool filesOpen[2] = {0};
	char fileNames[2][20];
	uint8_t fileIndex = 0;
	FIL* chosenFile;
	BYTE buff[BUFFER_SIZE + 1];
	FolderItems result;
	FatfsCommand command;
	uint8_t status;

	char path[MAX_FILEPATH];

	for(;;){
		if(xQueueReceive(s4436572_QueueFatfsCommand, &command, 0)){
			//console_log(DEBUG, "Queue receive fatfs type=%d name=%s\r\n", command.action, command.name);
			switch(command.action){
				case READ_FILE:
					chosenFile = NULL;
					for(uint8_t i=0; i<2; i++){
						if(strcmp(fileNames[i], command.name) == 0 && filesOpen[i]){
							chosenFile = &file[i];
						}
					}
					if(chosenFile == NULL){
						freeFileHandle = false;
						for(uint8_t i=0; i<2; i++){
							if(!filesOpen[i]){
								freeFileHandle = true;
								chosenFile = &file[i];
								status = s4436572_hal_fatfs_open_file(command.name, chosenFile, FA_READ);
								if(status == 0){
									strcpy(fileNames + i, command.name);
									filesOpen[i] = true;
									fileIndex = i;
								}
								break;
							}
						}
						if(!freeFileHandle){
							console_log(ERR, "Failed to open file %s, no file handles left\r\n", command.name);
						}
					}

					if(status == 0){
						console_log(LOG, "Contents of %s:\r\n", command.name);
						memset(buff, 0, BUFFER_SIZE + 1);
						while(BUFFER_SIZE == s4436572_hal_fatfs_read_file(chosenFile, buff, BUFFER_SIZE)){
							console_log(NONE, "%s", buff);
							memset(buff, 0, BUFFER_SIZE + 1);
						}
						console_log(NONE, "%s\r\n", buff);
						s4436572_hal_fatfs_close_file(chosenFile);
						filesOpen[fileIndex] = 0;
						console_log(LOG, "End of file\r\n");
					}else{
						console_log(ERR, "Failed to open file %s\r\n", command.name);
					}
					break;
				case WRITE_FILE:
					chosenFile = NULL;
					for(uint8_t i=0; i<2; i++){
						if(strcmp(fileNames[i], command.name) == 0 && filesOpen[i]){
							chosenFile = &file[i];
						}
					}
					if(chosenFile == NULL){
						freeFileHandle = false;
						for(uint8_t i=0; i<2; i++){
							if(!filesOpen[i]){
								freeFileHandle = true;
								chosenFile = &file[i];
								status = s4436572_hal_fatfs_open_file(command.name, chosenFile, FA_OPEN_APPEND | FA_WRITE);
								if(status == 0){
									strcpy(fileNames + i, command.name);
									filesOpen[i] = true;
									fileIndex = i;
								}
								break;
							}
						}
						if(!freeFileHandle){
							console_log(ERR, "Failed to open file %s, no file handles left\r\n", command.name);
						}
					}
					
					if(status == 0){
						s4436572_hal_fatfs_write_file(chosenFile, (BYTE*) command.name2, 70);	
					}else{
						console_log(ERR, "Failed to open file %s\r\n", command.name);
					}
					break;
				
				case CLOSE_FILE: 
					chosenFile = NULL;
					for(uint8_t i=0; i<2; i++){
						if(strcmp(fileNames[i], command.name) == 0 && filesOpen[i]){
							chosenFile = &file[i];
							fileIndex = i;
						}
					}
					if(chosenFile != NULL){
						s4436572_hal_fatfs_close_file(chosenFile);
						filesOpen[fileIndex] = 0;
					}
					break;
				case CREATE_FILE:
					status = s4436572_hal_fatfs_create_file(command.name);
					if(status == 0){
						console_log(LOG, "Created file %s\r\n", command.name);
					}else{
						console_log(ERR, "Failed to create file %s\r\n", command.name);
					}
					break;

				case DELETE_FILE:
					status = s4436572_hal_fatfs_delete_file(command.name);
					if(status == 0){
						console_log(LOG, "Deleted file %s\r\n", command.name);
					}else{
						console_log(ERR, "Failed to delete file %s\r\n", command.name);
					}
					break;
				
				case MOVE_FILE:
					status = s4436572_hal_fatfs_move_file(command.name, command.name2);
					if(status == 0){
						console_log(LOG, "Moved file %s to %s\r\n", command.name, command.name2);
					}else{
						console_log(ERR, "Failed to move file %s to %s\r\n", command.name, command.name2);
					}
					break;

				case LIST_DIR:
					status = s4436572_hal_fatfs_list_folder(&result);
					if(status == 0){
						s4436572_hal_fatfs_get_path(path);
						uint32_t free, total;
						s4436572_hal_fatfs_getfree(&free, &total);
						console_log(LOG, "Disk usage: %luKB total, %luKB free\r\n", total/1000, free/1000);
						console_log(LOG, "Contents of :%s\r\n", path);
						console_log(NONE, "  Name  \tSize\tTime\tAttrib\r\n");
						for(uint8_t i=0; i<result.count; i++){
							console_log(NONE, "%10s\t%d\t%d\t%d\r\n", result.items[i].name, result.items[i].size, result.items[i].time, result.items[i].type);
						}
					}else{
						console_log(ERR, "Failed to list current directory\r\n");
					}
					break;

				case CREATE_DIR:
					status = s4436572_hal_fatfs_create_folder(command.name);
					if(status == 0){
						console_log(LOG, "Created folder %s\r\n", command.name);
					}else{
						console_log(ERR, "Failed to create folder %s\r\n", command.name);
					}
					break;

				case DELETE_DIR:
					status = s4436572_hal_fatfs_delete_folder(command.name);
					if(status == 0){
						console_log(LOG, "Deleted folder %s\r\n", command.name);
					}else{
						console_log(ERR, "Failed to delete folder %s\r\n", command.name);
					}
					break;
				case CHANGE_DIR:
					status = s4436572_hal_fatfs_change_folder(command.name);
					if(status == 0){
						s4436572_hal_fatfs_get_path(path);
						console_log(LOG, ":%s>\r\n", path);
					}else{
						console_log(ERR, "Failed to change to directory %s\r\n", command.name);
					}
					break;
				case FORMAT:
					status = s4436572_hal_fatfs_format();
					if(status == 0){
						console_log(LOG, "Formatted disk\r\n");
					}else{
						console_log(ERR, "Failed to format\r\n");
					}
					break;
			}
		}
		vTaskDelay(10);
	}
}
