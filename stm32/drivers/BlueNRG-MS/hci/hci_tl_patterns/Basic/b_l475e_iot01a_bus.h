/**
  ******************************************************************************
  * @file           : b_l475e_iot01a_bus.h
  * @brief          : header file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef B_L475E_IOT01A_BUS_H
#define B_L475E_IOT01A_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "b_l475e_iot01a_conf.h"
#include "b_l475e_iot01a_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_L475E_IOT01A
  * @{
  */

/** @defgroup B_L475E_IOT01A_BUS B_L475E_IOT01A BUS
  * @{
  */

/** @defgroup B_L475E_IOT01A_BUS_Exported_Constants B_L475E_IOT01A BUS Exported Constants
  * @{
  */

#ifndef BUS_SPI1_POLL_TIMEOUT
  #define BUS_SPI1_POLL_TIMEOUT                   0x1000
#endif
/* SPI3 Baud rate in bps  */
#ifndef BUS_SPI3_BAUDRATE   
   #define BUS_SPI3_BAUDRATE   10000000U /* baud rate of SPIn = 10 Mbps*/
#endif

/**
  * @}
  */

/** @defgroup B_L475E_IOT01A_BUS_Private_Types B_L475E_IOT01A BUS Private types
  * @{
  */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1) 
typedef struct
{
  pSPI_CallbackTypeDef  pMspInitCb;
  pSPI_CallbackTypeDef  pMspDeInitCb;
}BSP_SPI3_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */
  
/** @defgroup B_L475E_IOT01A_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */ 
extern SPI_HandleTypeDef hspi3;	
/**
  * @}
  */

/** @addtogroup B_L475E_IOT01A_BUS_Exported_Functions
  * @{
  */    

/* BUS IO driver over SPI Peripheral */
HAL_StatusTypeDef MX_SPI3_Init(SPI_HandleTypeDef* hspi);
int32_t BSP_SPI3_Init(void);
int32_t BSP_SPI3_ReInit(void);
int32_t BSP_SPI3_DeInit(void);
int32_t BSP_SPI3_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI3_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI3_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);

int32_t BSP_GetTick(void);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI3_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI3_RegisterMspCallbacks (BSP_SPI3_Cb_t *Callbacks);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI3_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI3_RegisterMspCallbacks (BSP_SPI3_Cb_t *Callbacks);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1)  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* B_L475E_IOT01A_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
