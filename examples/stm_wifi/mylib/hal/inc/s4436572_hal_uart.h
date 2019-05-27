/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_hal_uart.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   UART HAL library
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_uart_init() - Setup UART
 * s4436572_hal_uart_deinit() - Deinitialise UART
 * s4436572_hal_uart_write(message) - Write to UART
 * s4436572_hal_uart_read(message, length) - Read from UART
 ******************************************************************************
 */
#ifndef S4436572_HAL_UART_H
#define S4436572_HAL_UART_H
#include "stm32l4xx.h"

void s4436572_hal_uart_init();
void s4436572_hal_uart_deinit();
HAL_StatusTypeDef hal_uart_send(const char* format, ...);
HAL_StatusTypeDef hal_uart_register_rx(void (*rxCallback)(char*));
uint16_t s4436572_hal_uart_write(const char* message);
uint16_t s4436572_hal_uart_read(char* message, int length);
#endif