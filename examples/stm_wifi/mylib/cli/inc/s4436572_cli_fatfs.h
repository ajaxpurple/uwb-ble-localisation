/**
 ******************************************************************************
 * @file    mylib/cli/s4436572_cli_fatfs.h
 * @author  Rohan Malik - 44365721
 * @date    25042019
 * @brief   Fat filesystem RTOS commands
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_cli_fatfs_init() - initialise fatfs CLI
 * s4436572_cli_fatfs_deinit() - deinitialise fatfs CLI
 ******************************************************************************
 */

#ifndef S4436572_CLI_FATFS_H
#define S4436572_CLI_FATFS_H

void s4436572_cli_fatfs_init(void);
void s4436572_cli_fatfs_deinit(void);
#endif