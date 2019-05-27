#include "s4436572_hal_imu.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_magneto.h"

void s4436572_hal_imu_init(){
    BSP_ACCELERO_Init();
    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
}
void s4436572_hal_imu_deinit(){
    BSP_ACCELERO_DeInit();
    BSP_MAGNETO_DeInit();
    BSP_GYRO_DeInit();
}

XYZValues s4436572_hal_accel_read(){
    int16_t xyzValues[3];
    BSP_ACCELERO_AccGetXYZ(xyzValues);
    return (XYZValues){.x=xyzValues[0], .y=xyzValues[1], .z=xyzValues[2]};
}

XYZValues s4436572_hal_magnet_read(){
    int16_t xyzValues[3];
    BSP_MAGNETO_GetXYZ(xyzValues);
    return (XYZValues){.x=xyzValues[0], .y=xyzValues[1], .z=xyzValues[2]};
}

XYZValues s4436572_hal_gyro_read(){
    int16_t xyzValues[3];
    BSP_GYRO_GetXYZ(xyzValues);
    return (XYZValues){.x=xyzValues[0], .y=xyzValues[1], .z=xyzValues[2]};
}