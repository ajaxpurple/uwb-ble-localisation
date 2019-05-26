/**
 ******************************************************************************
 * @file    mylib/inc/s4436572_os_printf.h
 * @author  Rohan Malik - 44365721
 * @date    11032019
 * @brief   Printf with colors
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_printf_init() - intialise printf/UART
 * s4436572_hal_printf_deinit() - deintialise printf/UART
 * debug_printf() - unsafe printf
 ******************************************************************************
 */

#ifndef S4436572_HAL_PRINTF_H
#define S4436572_HAL_PRINTF_H
#define MESSAGE_LENGTH 256 //Maximum buffer size

typedef enum{
	DARK_RED, LIGHT_RED, 
    DARK_GREEN, LIGHT_GREEN, 
    DARK_YELLOW, LIGHT_YELLOW, 
    DARK_BLUE, LIGHT_BLUE, 
    DARK_MAGENTA, LIGHT_MAGENTA,
    DARK_CYAN, LIGHT_CYAN
}Color;

void s4436572_hal_printf_init(void);
void s4436572_hal_printf_deinit(void);
void set_color(Color color);
void reset_color();
int debug_getchar(void);
int debug_putchar(int c);
uint16_t debug_printf(const char* format, ...);
#endif
