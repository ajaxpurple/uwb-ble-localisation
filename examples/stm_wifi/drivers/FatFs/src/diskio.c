/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2017        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2017, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/**
  ******************************************************************************
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "ff_gen_drv.h"
#include "stm32l475e_iot01_qspi.h"
#include <string.h>
#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (BYTE pdrv){
  if(pdrv > 0){
    return STA_NODISK;
  }
  uint8_t status = BSP_QSPI_GetStatus();
  if(status == QSPI_OK){
    return 0;
  }else if(status == QSPI_ERROR){
    return STA_NOINIT;
  }else{
    return STA_PROTECT;
  }
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (BYTE pdrv){
  if(pdrv > 0){
    return STA_NODISK;
  }
  uint8_t stat = BSP_QSPI_Init();
  if(stat == QSPI_OK){
    return 0;
  }else{
    return STA_NOINIT;
  }
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (BYTE pdrv, BYTE *buff,	DWORD sector,	UINT count){
  if(pdrv > 0){
    return RES_PARERR;
  }

  DRESULT res;
  res = BSP_QSPI_Read((uint8_t*) buff, sector*MX25R6435F_SECTOR_SIZE, count*MX25R6435F_SECTOR_SIZE);
  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count) {
  if(pdrv > 0){
    return RES_PARERR;
  }
  DRESULT res;
  for(UINT i=0; i<count; i++){
    BSP_QSPI_Erase_Sector(sector + i);
  }
  while(BSP_QSPI_GetStatus() == QSPI_BUSY){
    HAL_Delay(10);
  }
  res = BSP_QSPI_Write((uint8_t*) buff, sector*MX25R6435F_SECTOR_SIZE, count*MX25R6435F_SECTOR_SIZE); 
  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff){
  DWORD sectorCount;
  WORD sectorSize;
  DWORD blockSize;
  DWORD addrRange[2];
  DRESULT status = RES_OK;
  if(pdrv > 0){
    return RES_PARERR;
  }
  switch(cmd){
    case CTRL_SYNC:
      status = BSP_QSPI_GetStatus();
      while(status == QSPI_BUSY){
        HAL_Delay(10);
        status = BSP_QSPI_GetStatus();
      }
      if(status != QSPI_OK){
        status = RES_ERROR;
      }
      break;
    case GET_SECTOR_COUNT:
      sectorCount = MX25R6435F_FLASH_SIZE/MX25R6435F_SECTOR_SIZE;
      memcpy(buff, &sectorCount, sizeof(DWORD));
      break;
    case GET_SECTOR_SIZE:
      sectorSize = MX25R6435F_SECTOR_SIZE;
      memcpy(buff, &sectorSize, sizeof(WORD));
      break;
    case GET_BLOCK_SIZE:
      blockSize = MX25R6435F_BLOCK_SIZE/MX25R6435F_SECTOR_SIZE;
      memcpy(buff, &blockSize, sizeof(DWORD));
      break;
    case CTRL_TRIM:
      memcpy(addrRange, buff, 2*sizeof(DWORD));
      for(DWORD i=addrRange[0]; i<=addrRange[1]; i++){
        status = BSP_QSPI_Erase_Sector(i);
      }
      break;
  }
  return status;
}
#endif /* _USE_IOCTL == 1 */

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
__weak DWORD get_fattime (void)
{
  return HAL_GetTick()/1000;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

