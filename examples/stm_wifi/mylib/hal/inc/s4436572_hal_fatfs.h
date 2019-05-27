/**
 ******************************************************************************
 * @file    mylib/s4436572_hal_fatfs.h
 * @author  Rohan Malik - 44365721
 * @date    17032019
 * @brief   LED HAL library
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
void s4436572_hal_led_init() - Initialise LEDs
void s4436572_hal_led_toggle(int led) - Toggle LED
void s4436572_hal_led_write(int led, int value) - Write value to LED
void s4436572_hal_led_deinit() - De-initialise LEDs
 ******************************************************************************
 */
#ifndef FATFS_H
#define FATFS_H
#include "stm32l4xx.h"
#include "ff.h"


#define MAX_ITEM_COUNT  20
#define MAX_FILENAME    10
#define MAX_FILEPATH    30

typedef struct{
    uint8_t type;
    char name[MAX_FILENAME];
    uint16_t size;
    uint32_t time;
}Item;

typedef struct{
    uint8_t count;
    Item items[MAX_ITEM_COUNT];
}FolderItems;

uint8_t s4436572_hal_fatfs_init();
uint8_t s4436572_hal_fatfs_getfree(uint32_t* free, uint32_t* total);
uint8_t s4436572_hal_fatfs_format();
uint8_t s4436572_hal_fatfs_create_file(const char* name);
uint8_t s4436572_hal_fatfs_open_file(const char* name, FIL* file, BYTE mode);
uint8_t s4436572_hal_fatfs_read_file(FIL* file, BYTE* data, UINT length);
uint8_t s4436572_hal_fatfs_write_file(FIL* file, BYTE* data, UINT length);
uint8_t s4436572_hal_fatfs_close_file(FIL* file);
uint8_t s4436572_hal_fatfs_delete_file(const char* name);
uint8_t s4436572_hal_fatfs_move_file(const char* name, const char* target);
uint8_t s4436572_hal_fatfs_get_path(char* path);
uint8_t s4436572_hal_fatfs_create_folder(const char* name);
uint8_t s4436572_hal_fatfs_change_folder(const char* name);
uint8_t s4436572_hal_fatfs_list_folder(FolderItems* result);
uint8_t s4436572_hal_fatfs_delete_folder(char* name);

#endif