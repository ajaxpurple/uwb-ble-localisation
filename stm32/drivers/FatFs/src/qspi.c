#include "stm32l4xx_hal.h"

QSPI_HandleTypeDef hqspi;
DMA_HandleTypeDef hdma_quadspi;

void qspi_gpio_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_QSPI_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* QUADSPI interrupt Init */
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
}

void qspi_init(){
  
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
  }
}

void qspi_read(){
  //HAL_QSPI_Receive();
}

void qspi_dma_init(){
  hdma_quadspi.Instance = DMA1_Channel5;
  hdma_quadspi.Init.Request = DMA_REQUEST_5;
  hdma_quadspi.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_quadspi.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_quadspi.Init.MemInc = DMA_MINC_ENABLE;
  hdma_quadspi.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_quadspi.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_quadspi.Init.Mode = DMA_NORMAL;
  hdma_quadspi.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_quadspi) != HAL_OK)
  {
    //Error_Handler();
  }

  __HAL_LINKDMA(&hqspi,hdma,hdma_quadspi);
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}