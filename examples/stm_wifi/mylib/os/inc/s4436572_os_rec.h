/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_rec.h
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Recording RTOS
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_os_record_init() - initialise recording
 * s4436572_os_record_deinit() - de-initialise recording
 ******************************************************************************
 */

#ifndef S4436572_OS_REC_H
#define S4436572_OS_REC_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <stdbool.h>

typedef enum{
    ACCEL,
    GYRO,
    MAGNET
}SensorType;

typedef struct{
    bool enabled;
    char filename[10];
}RecSetting;

typedef struct{
    RecSetting setting;
    SensorType sensorType;    
}RecCommand;

typedef struct{
    RecSetting accel;
    RecSetting gyro;
    RecSetting magnet;
}RecConfig;

extern QueueHandle_t s4436572_QueueRecCommand;

/* External function prototypes -----------------------------------------------*/
void s4436572_os_rec_init(void); //Initialise led
void s4436572_os_rec_deinit(void); //De-initialise led

#endif
