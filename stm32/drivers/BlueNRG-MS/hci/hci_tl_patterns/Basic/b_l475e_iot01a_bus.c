/**
  ******************************************************************************
  * @file           : b_l475e_iot01a_bus.h
  * @brief          : source file for the BSP BUS IO driver
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

/* Includes ------------------------------------------------------------------*/
#include "b_l475e_iot01a_bus.h"
#include "stm32l475xx.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_L475E_IOT01A
  * @{
  */

/** @defgroup B_L475E_IOT01A_BUS B_L475E_IOT01A BUS
  * @{
  */
  

/** @defgroup B_L475E_IOT01A_Private_Variables BUS Private Variables
  * @{
  */

SPI_HandleTypeDef hspi3;						

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)						
static uint32_t IsSPI3MspCbValid = 0;	
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */			
/**
  * @}
  */

/** @defgroup B_L475E_IOT01A_Private_FunctionPrototypes  Private Function Prototypes
  * @{
  */  

static void SPI3_MspInit(SPI_HandleTypeDef* hSPI); 
static void SPI3_MspDeInit(SPI_HandleTypeDef* hSPI);
#if (USE_CUBEMX_BSP_V2 == 1)
static uint32_t SPI_GetPrescaler( uint32_t clk_src_hz, uint32_t baudrate_mbps );
#endif

/**
  * @}
  */

/** @defgroup B_L475E_IOT01A_LOW_LEVEL_Private_Functions B_L475E_IOT01A LOW LEVEL Private Functions
  * @{
  */ 
  
/** @defgroup B_L475E_IOT01A_BUS_Exported_Functions B_L475E_IOT01A_BUS Exported Functions
  * @{
  */   

/* BUS IO driver over SPI Peripheral */
/*******************************************************************************
                            BUS OPERATIONS OVER SPI
*******************************************************************************/
/**
  * @brief  Initializes SPI HAL. 
  * @retval BSP status
  */
int32_t BSP_SPI3_Init(void) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  hspi3.Instance  = SPI3;
  if (HAL_SPI_GetState(&hspi3) == HAL_SPI_STATE_RESET) 
  { 
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* Init the SPI Msp */
    SPI3_MspInit(&hspi3);
#else
    if(IsSPI3MspCbValid == 0U)
    {
      if(BSP_SPI3_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif   
    
    /* Init the SPI */
    if (MX_SPI3_Init(&hspi3) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  } 

  return ret;
}

int32_t BSP_SPI3_ReInit(void)
{
  HAL_StatusTypeDef ret = 0;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    ret = -1;
  }
  return ret;
}

/**
  * @brief  DeInitializes SPI HAL.
  * @retval None
  * @retval BSP status
  */
int32_t BSP_SPI3_DeInit(void) 
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
  SPI3_MspDeInit(&hspi3);
#endif  
  /* DeInit the SPI*/
  if (HAL_SPI_DeInit(&hspi3) == HAL_OK) {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
}

/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Pointer to data buffer to send
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI3_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;
  
  if(HAL_SPI_Transmit(&hspi3, pData, Length, BUS_SPI1_POLL_TIMEOUT) == HAL_OK)
  {
      ret = BSP_ERROR_NONE;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Pointer to data buffer to receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI3_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;
  
  if(HAL_SPI_Receive(&hspi3, pData, Length, BUS_SPI1_POLL_TIMEOUT) == HAL_OK)
  {
      ret = BSP_ERROR_NONE;
  }
  return ret;
}

/**
  * @brief  Send and Receive data to/from SPI BUS (Full duplex)
  * @param  pData: Pointer to data buffer to send/receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI3_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_UNKNOWN_FAILURE;
  
  if(HAL_SPI_TransmitReceive(&hspi3, pTxData, pRxData, Length, BUS_SPI1_POLL_TIMEOUT) == HAL_OK)
  {
      ret = BSP_ERROR_NONE;
  }
  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)  
/**
  * @brief Register Default BSP SPI3 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_SPI3_RegisterDefaultMspCallbacks (void)
{

  __HAL_SPI_RESET_HANDLE_STATE(&hspi3);
  
  /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPINIT_CB_ID, SPI3_MspInit)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPDEINIT_CB_ID, SPI3_MspDeInit) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  IsSPI3MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}

/**
  * @brief BSP SPI3 Bus Msp Callback registering
  * @param Callbacks     pointer to SPI3 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_SPI3_RegisterMspCallbacks (BSP_SPI_Cb_t *Callbacks)
{
  /* Prevent unused argument(s) compilation warning */
  __HAL_SPI_RESET_HANDLE_STATE(&hspi3);  
 
   /* Register MspInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPINIT_CB_ID, Callbacks->pMspInitCb)  != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  /* Register MspDeInit Callback */
  if (HAL_SPI_RegisterCallback(&hspi3, HAL_SPI_MSPDEINIT_CB_ID, Callbacks->pMspDeInitCb) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  
  IsSPI3MspCbValid = 1;
  
  return BSP_ERROR_NONE;  
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick(void) {
  return HAL_GetTick();
}

/* SPI3 init function */ 



__weak HAL_StatusTypeDef MX_SPI3_Init(SPI_HandleTypeDef* hspi)
{
  HAL_StatusTypeDef ret = HAL_OK;
  hspi->Instance = SPI3;
  hspi->Init.Mode = SPI_MODE_MASTER;
  hspi->Init.Direction = SPI_DIRECTION_2LINES;
  hspi->Init.DataSize = SPI_DATASIZE_8BIT;
  hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi->Init.NSS = SPI_NSS_SOFT;
  hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial = 7;
  hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void SPI3_MspInit(SPI_HandleTypeDef* spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN SPI3_MspInit 0 */

  /* USER CODE END SPI3_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_SPI3_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI3 GPIO Configuration    
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI3_MspInit 1 */

  /* USER CODE END SPI3_MspInit 1 */
}

static void SPI3_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
  /* USER CODE BEGIN SPI3_MspDeInit 0 */

  /* USER CODE END SPI3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();
  
    /**SPI3 GPIO Configuration    
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI 
    */
    HAL_GPIO_DeInit(GPIOC,  GPIO_PIN_10 | GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN SPI3_MspDeInit 1 */

  /* USER CODE END SPI3_MspDeInit 1 */
}

#if (USE_CUBEMX_BSP_V2 == 1)
/**
  * @brief  Convert the SPI baudrate into prescaler.
  * @param  clock_src_hz : SPI source clock in HZ.
  * @param  baudrate_mbps : SPI baud rate in mbps.
  * @retval Prescaler dividor
  */
static uint32_t SPI_GetPrescaler( uint32_t clock_src_hz, uint32_t baudrate_mbps )
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;

  static const uint32_t baudrate[]=
  {
    SPI_BAUDRATEPRESCALER_2,  
    SPI_BAUDRATEPRESCALER_4,      
    SPI_BAUDRATEPRESCALER_8,      
    SPI_BAUDRATEPRESCALER_16,     
    SPI_BAUDRATEPRESCALER_32,     
    SPI_BAUDRATEPRESCALER_64,     
    SPI_BAUDRATEPRESCALER_128,   
    SPI_BAUDRATEPRESCALER_256, 
  };
  
  while( spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];
    if (++divisor > 7)
      break;
      
    spi_clk= ( spi_clk >> 1);
  }
  
  return presc;
}
#endif
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
