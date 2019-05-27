/**
 ******************************************************************************
 * @file    mylib/s4436572_hal_led.c
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
#include "s4436572_hal_fatfs.h"
#include "stm32l4xx_hal.h"
#include "s4436572_hal_board.h"
#include "stm32l475e_iot01_qspi.h"
#include "s4436572_hal_printf.h"
#include "ff.h"
#include "ffconf.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MAX_DEPTH 10
FATFS FatFs;
BYTE work[_MAX_SS];
FolderItems dirItems;
char nameStack[MAX_DEPTH][MAX_FILENAME];
uint8_t stackDepth = 0;

uint8_t s4436572_hal_fatfs_format(){
    return f_mkfs("", FM_FAT, _MAX_SS, work, sizeof work);
}

uint8_t s4436572_hal_fatfs_getfree(uint32_t* free, uint32_t* total){
    DWORD fre_clust, fre_sect, tot_sect;
    FATFS* pFatFs = &FatFs;
    uint8_t retValue = f_getfree("", &fre_clust, &pFatFs);
    tot_sect = (FatFs.n_fatent - 2) * FatFs.csize;
    fre_sect = fre_clust * FatFs.csize;
    *free = fre_sect*FatFs.ssize;
    *total = tot_sect*FatFs.ssize;
    return retValue;
}

uint8_t s4436572_hal_fatfs_init(){
    BSP_QSPI_MspInit();
    FRESULT status;
    //f_mkfs("", FM_FAT, _MAX_SS, work, sizeof work);
    if(status = f_mount(&FatFs, "", 1), status != FR_OK){
        status = f_mkfs("", FM_FAT, _MAX_SS, work, sizeof work);
        if(status == FR_OK){
            status = f_mount(&FatFs, "", 1);
        }
    }
    return status;
}

uint8_t s4436572_hal_fatfs_create_file(const char* name){
    FIL file;
    FRESULT status;
    status = f_open(&file, name, FA_CREATE_NEW);
    if(status == FR_OK){
        status = f_close(&file);
    }
    return status;
}

uint8_t s4436572_hal_fatfs_open_file(const char* name, FIL* file, BYTE mode){
    return f_open(file, name, mode);
}

uint8_t s4436572_hal_fatfs_read_file(FIL* file, BYTE* data, UINT length){
   UINT bytes_read;
   f_read(file, data, length, &bytes_read);
   return bytes_read;
}

uint8_t s4436572_hal_fatfs_write_file(FIL* file, BYTE* data, UINT length){
   UINT bytes_written;
   UINT new_length;
   if(strlen(data) < length){
       new_length = strlen(data);
   }else{
       new_length = length;
   }
   f_write(file, data, new_length, &bytes_written);
   return bytes_written;
}

uint8_t s4436572_hal_fatfs_close_file(FIL* file){
   return f_close(file);
}

uint8_t s4436572_hal_fatfs_delete_file(const char* name){
    return f_unlink(name);
}

uint8_t s4436572_hal_fatfs_move_file(const char* name, const char* target){
    char new_path[MAX_FILEPATH];
    sprintf(new_path, "%s/%s", target, name);
    return f_rename(name, new_path);
}

uint8_t s4436572_hal_fatfs_create_folder(const char* name){
    return f_mkdir(name);
}

uint8_t s4436572_hal_fatfs_change_folder(const char* name){
    return f_chdir(name);
}

uint8_t s4436572_hal_fatfs_get_path(char* path){
    return f_getcwd(path, MAX_FILEPATH);
}

uint8_t s4436572_hal_fatfs_list_folder(FolderItems* result){
    DIR currentDir;
    FILINFO fileInfo;
    TCHAR path[MAX_FILEPATH];
    FRESULT status;
    status = f_getcwd(path, MAX_FILEPATH);
    if(status == FR_OK){
        status = f_opendir(&currentDir, path);
        if(status == FR_OK){
            result->count = 0;
            
            while(status = f_readdir(&currentDir, &fileInfo), result->count < MAX_ITEM_COUNT && status == FR_OK && fileInfo.fname[0] != 0){
                strcpy(result->items[result->count].name, fileInfo.fname);
                result->items[result->count].size = fileInfo.fsize;
                result->items[result->count].time = (fileInfo.fdate<<16) | fileInfo.ftime;
                result->items[result->count].type = fileInfo.fattrib;
                result->count++;
            }    
            if(result->count == MAX_ITEM_COUNT){
                return 1;
            }
            status = f_closedir(&currentDir);
        }
    }
    return status;
}

uint8_t s4436572_hal_fatfs_delete_folder(char* folderName){
    FRESULT status;
    status = f_chdir(folderName);
    strcpy(nameStack[stackDepth], folderName);
    if(status == FR_OK){
        status = s4436572_hal_fatfs_list_folder(&dirItems);
        if(status == FR_OK){
            while(dirItems.count > 0){
                if(dirItems.items[0].type & AM_DIR){
                    stackDepth++;
                    status = s4436572_hal_fatfs_delete_folder(dirItems.items[0].name);
                    stackDepth--;
                    if(status != FR_OK){
                        return status;
                    }
                    if(status != FR_OK){
                        return status;
                    }
                }else{
                    status = f_unlink(dirItems.items[0].name);
                    if(status != FR_OK){
                        return status;
                    }
                }
                status = s4436572_hal_fatfs_list_folder(&dirItems);
            }
            if(status == FR_OK){
                status = f_chdir("..");            
                if(status == FR_OK){
                    strcpy(folderName, nameStack[stackDepth]);
                    status = f_unlink(folderName);
                }
            }
        }
    }
    return status;
}