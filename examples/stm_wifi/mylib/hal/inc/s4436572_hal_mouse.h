#ifndef S4436572_HAL_MOUSE_H
#define S4436572_HAL_MOUSE_H
#include "stm32l4xx.h"
void s4436572_hal_mouse_init();
void s4436572_hal_mouse_move(int8_t dx, int8_t dy);
void s4436572_hal_mouse_down();
void s4436572_hal_mouse_up();
#endif