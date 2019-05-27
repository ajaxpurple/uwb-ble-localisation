/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_imu.h
 * @author  Rohan Malik - 44365721
 * @date    31032019
 * @brief   Inertial measurement unit HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_imu_init() - Setup Bluetooth
 * s4436572_hal_imu_deinit() - Deinitialise Bluetooth
 * s4436572_hal_accel_read(uint16_t* x, uint16_t* y, uint16_t* z) - Read accelerometer
 * s4436572_hal_magnet_read(uint16_t* x, uint16_t* y, uint16_t* z) - Read magnetometer
 * s4436572_hal_gyro_read(uint16_t* x, uint16_t* y, uint16_t* z) - Read gyroscope
 ******************************************************************************
 */
#ifndef S4436572_HAL_IMU_H
#define S4436572_HAL_IMU_H
#include "stm32l4xx.h"

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
}XYZValues;

void s4436572_hal_imu_init();
void s4436572_hal_imu_deinit();
XYZValues s4436572_hal_accel_read();
XYZValues s4436572_hal_magnet_read();
XYZValues s4436572_hal_gyro_read();
#endif