/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_fatfs.h
 * @author  Rohan Malik - 44365721
 * @date    25042019
 * @brief   FATFS RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_fatfs_init() - initialise fatfs
 * s4436572_os_fatfs_deinit() - de-initialise fatfs
 ******************************************************************************
 */

#ifndef S4436572_OS_FATFS_H
#define S4436572_OS_FATFS_H

#include "FreeRTOS.h"
#include "queue.h"

typedef enum{
    READ_FILE,
    WRITE_FILE,
    CREATE_FILE,
    CLOSE_FILE,
    DELETE_FILE,
    MOVE_FILE,
    LIST_DIR,
    CREATE_DIR,
    DELETE_DIR,
    CHANGE_DIR,
    FORMAT
}FatfsAction;

typedef struct{
	FatfsAction action;
    char name[20];
    char name2[80];
}FatfsCommand;

extern QueueHandle_t s4436572_QueueFatfsCommand;

/* External function prototypes -----------------------------------------------*/
void s4436572_os_fatfs_init(void); //Initialise fatfs
void s4436572_os_fatfs_deinit(void); //De-initialise fatfs

#endif
