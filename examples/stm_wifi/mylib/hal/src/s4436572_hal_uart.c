/**
 ******************************************************************************
 * @file    mylib/s4436572_hal_uart.c
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   UART HAL library
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_uart_init() - Setup UART
 * s4436572_hal_uart_deinit() - Deinitialise UART
 * s4436572_hal_uart_write(message) - Write to UART
 * s4436572_hal_uart_read(message, length) - Read from UART
 ******************************************************************************
 */
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"

#include <string.h>
#include "s4436572_hal_led.h"
#include "s4436572_hal_board.h"

#define TIMEOUT   15
#define BAUD_RATE 115200
#define INPUT_SIZE 100
#define OUTPUT_SIZE 200

uint8_t byte; //Byte received
static UART_HandleTypeDef huart;
char inputString[INPUT_SIZE];
char outputString[OUTPUT_SIZE];
int inputIndex = 0;
int outputIndex = 0;

void (*commandCallback)(char*);//Called when command has been received

/**
 * @brief Send character via UART
 * @param letter - Character to send
 * @return Status indicating whether sending was successful
 */
HAL_StatusTypeDef hal_uart_send_char(uint8_t letter) {
	return HAL_UART_Transmit(&huart, &letter, 1, 1); //Transmit letter
}

void s4436572_hal_uart_init(){
    /* Enable GPIO clock */
    UART_GPIO_PORT_CLK_ENABLE();

    /* Enable USART clock */
    UART_CLK_ENABLE();

    /* Configure USART Tx as alternate function */
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.Pin = UART_TX_GPIO_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Alternate = UART_AF;
    HAL_GPIO_Init(UART_GPIO_PORT, &gpio_init_structure);

    /* Configure USART Rx as alternate function */
    gpio_init_structure.Pin = UART_RX_GPIO_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Alternate = UART_AF;
    HAL_GPIO_Init(UART_GPIO_PORT, &gpio_init_structure);

    huart.Instance = UART; 
    huart.Init.BaudRate = BAUD_RATE;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_8;
    huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
    HAL_UART_Init(&huart);
    HAL_NVIC_SetPriority(UART_IRQ, 5, 0);
    HAL_NVIC_EnableIRQ(UART_IRQ);
}

void s4436572_hal_uart_deinit(){
    /* USART configuration */
    HAL_UART_DeInit(&huart);

    /* Enable USART clock */
    UART_CLK_DISABLE();
}

uint16_t s4436572_hal_uart_write(const char* message){
  uint16_t length = strlen(message);
  strcpy(outputString, message);
  outputString[length] = 0;
  HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&huart, (uint8_t*)message, length);
  return (status == HAL_OK ? length : 0);
}

uint16_t s4436572_hal_uart_read(char* message, int length){
  HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart, (uint8_t*)message, length);
  return (status == HAL_OK ? length : 0);
}

/**
 * @brief Interrupt called when a byte is received.
 * 		Attempts to form commands and calls a function to handle the command
 * @param byte - The byte received via an interrupt
 */
void on_byte_receive(uint8_t rxChar) {

  /* Echo character */
  hal_uart_send_char(rxChar);
  /* Process only if return is received. */
  if (rxChar == '\r') {
    hal_uart_send_char('\n');
    inputString[inputIndex] = '\0';
    (*commandCallback)(inputString);
    inputIndex = 0;
    inputString[inputIndex] = '\0';

  } else if(rxChar == '\b' || rxChar == 127) {
    /* Backspace was pressed.*/
    if(inputIndex > 0) {
      inputIndex--;
      inputString[inputIndex] = '\0';
      s4436572_hal_uart_write("\b \b");
    }
  } else {
    if(inputIndex < INPUT_SIZE - 1) {
      inputString[inputIndex] = rxChar;
      inputIndex++;
      inputString[inputIndex] = '\0';
    }
  }
}

/**
 * @brief Register a command callback that is called when a command is received via UART
 * @param callback - The callback function to call
 * @return Status indicating whether setting up reception interrupt was successful
 */
HAL_StatusTypeDef hal_uart_register_rx(void (*callback)(char*)) {
	commandCallback = callback; //Register command callback
	return HAL_UART_Receive_IT(&huart, &byte, 1); //Receive character with interrupt
}

/**
 * @brief USART interrupt handler called when an interrupt is received (TX or RX)
 */
void UART_IRQ_HANDLER(void) {
	HAL_UART_IRQHandler(&huart);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle){
  if(UartHandle->Instance == UART){
    if(__HAL_UART_GET_FLAG(UartHandle, UART_FLAG_ORE) != RESET){
      __HAL_UART_CLEAR_FLAG(UartHandle, UART_FLAG_ORE);
    }
  } 
}

/**
 *  @brief This callback is called by the HAL_UART_IRQHandler when
 *  		the given number of bytes are received
 * 	@param huart - The UART handle that triggered the interrupt callback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == UART) {
		on_byte_receive(byte); //Parse byte
		HAL_UART_Receive_IT(huart, &byte, 1); //Receive next byte in interrupt'    
	}
}

/**
 *  @brief This callback is called by the HAL_UART_IRQHandler when
 *  		the given number of bytes are received
 * 	@param huart - The UART handle that triggered the interrupt callback
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	outputString[0] = '\0';
}
