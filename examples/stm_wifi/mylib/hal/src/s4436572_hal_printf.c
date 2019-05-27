/**
 ******************************************************************************
 * @file    mylib/s4436572_hal_printf.c
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   Printf HAL library
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_printf_init() - intialise printf/UART
 * s4436572_hal_printf_deinit() - deintialise printf/UART
 * debug_printf() - unsafe printf
 * debug_puts() - unsafe puts
 * debug_getchar() - unsafe getchar
 * debug_putchar() - unsafe putchar
 ******************************************************************************
 */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32l475xx.h"
#include "s4436572_hal_printf.h"
#include "s4436572_hal_uart.h"

char debugMessage[MESSAGE_LENGTH];
extern void s4436572_hal_printf_init(void){
	s4436572_hal_uart_init();
}

void s4436572_hal_printf_deinit(void){
  s4436572_hal_uart_deinit();
}


int debug_putchar(int c){
  char charString[2];
  charString[0] = c;
  charString[1] = '\0';
  s4436572_hal_uart_write(charString);
  return c;
}

int debug_getchar(void){
  char c = 0;
  s4436572_hal_uart_read(&c, 1);
  return (int) c;
}

int debug_puts(const char* str){
  return s4436572_hal_uart_write(str);
}

uint16_t debug_printf(const char* format, ...){
    va_list arguments;
    va_start(arguments, format);
    vsprintf(debugMessage, format, arguments);
    va_end(arguments);
    return s4436572_hal_uart_write(debugMessage);
}

void set_color(Color color){
	debug_printf("\033[%d;%dm", color & 1, (color>>1) + 31);
}

void reset_color(){
	debug_printf("\033[0m");
}

int _read(int file, char* ptr, int len){
  return s4436572_hal_uart_read(ptr, len);
}
