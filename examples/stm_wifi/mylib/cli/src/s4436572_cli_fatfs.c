/**
 ******************************************************************************
 * @file    mylib/s4436572_cli_fatfs.c
 * @author  Rohan Malik - 44365721
 * @date    14032019
 * @brief   Fat Filesystem RTOS CLI functions
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_cli_fatfs_init() � initialise fatfs CLI
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s4436572_cli_fatfs.h"
#include "s4436572_os_fatfs.h"
#include "s4436572_os_rec.h"
#include "s4436572_os_cli.h"
#include "s4436572_os_log.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "argtable2.h"

#define TICK_COUNT 10 //Tick count for queues
#define NUM_COMMANDS 5

int file_argtable(int argc, char *argv[]);
int dir_argtable(int argc, char *argv[]);
int cd_argtable(int argc, char *argv[]);
int rec_argtable(int argc, char *argv[]);
int reset_argtable(int argc, char *argv[]);

Command command[5];
FatfsCommand diskOp;
RecCommand record;


/**
  * @brief  Initialise CLI fatfs.
  * @param  None
  * @retval None
*/
extern void s4436572_cli_fatfs_init(void) {
    command[0] = (Command){.name="file", .func=&file_argtable};
    command[1] = (Command){.name="dir", .func=&dir_argtable};
    command[2] = (Command){.name="cd", .func=&cd_argtable};
    command[3] = (Command){.name="rec", .func=&rec_argtable};
    command[4] = (Command){.name="reset", .func=&reset_argtable};
    for(uint8_t i=0; i<NUM_COMMANDS; i++){
        xQueueSendToBack(s4436572_QueueAddCommand, command + i, TICK_COUNT);   
    }     
}

/**
  * @brief  Callback for fatfs file command
  * @param  argc, argv
  * @retval None
*/
int file_argtable(int argc, char *argv[]){

    /* global arg_xxx structs */
    struct arg_lit *help;
    struct arg_str *create, *view, *delete, *move;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "Display this help"),
        create    = arg_str0("c", "create", "<filename>", "Create file"),
        view   	= arg_str0("v", "view", "<filename>", "View file"),
        delete  = arg_str0("d", "delete", "<filename>", "Delete file"),
        move 	= arg_strn("m", "move", "<filename> <directory>", 0, 2, "Move file"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);
    if(create->count > 0){
        diskOp.action = CREATE_FILE;  
        strcpy(diskOp.name, create->sval[0]);
        console_log(DEBUG, "Queue add fatfs create file %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }else if(view->count > 0){
        diskOp.action = READ_FILE;
        strcpy(diskOp.name, view->sval[0]);
        console_log(DEBUG, "Queue add fatfs view file %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);;
        return 0;
    }else if(delete->count > 0){
        diskOp.action = DELETE_FILE;  
        strcpy(diskOp.name, delete->sval[0]);
        console_log(DEBUG, "Queue add fatfs delete file %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }else if(move->count > 0){
        diskOp.action = MOVE_FILE;  
        strcpy(diskOp.name, move->sval[0]);
        strcpy(diskOp.name2, move->sval[1]);
        console_log(DEBUG, "Queue add fatfs move %s, to %s\r\n", diskOp.name, diskOp.name2);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "File operations\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
}


/**
  * @brief  Callback for fatfs directory command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int dir_argtable(int argc, char *argv[]){

    /* global arg_xxx structs */
    struct arg_lit *help, *list;
    struct arg_str *create, *delete;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "Display this help"),
        list   	= arg_lit0("l", "list", "List current directory"),
        create  = arg_str0("c", "create", "<filename>", "Create directory"),
        delete  = arg_str0("d", "delete", "<filename>", "Delete directory"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);
    if(list-> count > 0){
        diskOp.action = LIST_DIR;  
        console_log(DEBUG, "Queue add fatfs list\r\n");
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }else if(create->count > 0){
        diskOp.action = CREATE_DIR;
        strcpy(diskOp.name, create->sval[0]);
        console_log(DEBUG, "Queue add fatfs create %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);;
        return 0;
    }else if(delete->count > 0){
        diskOp.action = DELETE_DIR;  
        strcpy(diskOp.name, delete->sval[0]);
        console_log(DEBUG, "Queue add fatfs delete %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }else{
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "File operations\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }
}


/**
  * @brief  Callback for record command
  * @param  writebuffer, writebuffer length and command strength
  * @retval None
*/
int rec_argtable(int argc, char *argv[]){

    /* global arg_xxx structs */
    struct arg_lit *help;
    struct arg_str *enable, *disable;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "Display this help"),
        enable   = arg_strn("e", "enable", "<sensor name> <filename>", 2, 2, "Enable"),
        disable  = arg_strn("d", "disable", "<sensor name> <filename>", 2, 2, "Disable"),
        end     = arg_end(10),
    };

    int nerrors = arg_parse(argc,argv,argtable);
    if(enable->count > 1){
        record.setting.enabled = true; 
        switch(enable->sval[0][0]){
            case 'a': record.sensorType = ACCEL; break;
            case 'g': record.sensorType = GYRO; break;
            case 'm': record.sensorType = MAGNET; break;
        }
        strcpy(record.setting.filename, enable->sval[1]);
        console_log(DEBUG, "Queue add enable %d %s\r\n", record.sensorType, record.setting.filename);
        xQueueSendToBack(s4436572_QueueRecCommand, &record, TICK_COUNT);
        return 0;
    }else if(disable->count > 1){
        record.setting.enabled = false;
        switch(disable->sval[0][0]){
            case 'a': record.sensorType = ACCEL; break;
            case 'g': record.sensorType = GYRO; break;
            case 'm': record.sensorType = MAGNET; break;
        }
        strcpy(record.setting.filename, enable->sval[1]);
        console_log(DEBUG, "Queue add disable %d %s\r\n", record.sensorType, record.setting.filename);
        xQueueSendToBack(s4436572_QueueRecCommand, &record, TICK_COUNT);;
        return 0;
    }else if(help->count > 0){
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Log from sensors\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }else if (nerrors > 0){
        arg_print_errors(stdout, end, argv[0]);
        console_log(NONE, "\r");
        return nerrors;
    }
    return 0;
}


/**
  * @brief  Callback for fatfs change directory command
  * @param  argc, argv
  * @retval None
*/
int cd_argtable(int argc, char *argv[]){

    /* global arg_xxx structs */
    struct arg_lit *help;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "Display this help"),
        end     = arg_end(10),
    };

    arg_parse(argc,argv,argtable);
    if(help-> count > 0){
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Directory operations\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }else{
        diskOp.action = CHANGE_DIR;
        strcpy(diskOp.name, argv[1]);
        console_log(DEBUG, "Queue add command change to directory %s\r\n", diskOp.name);
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }
}

int reset_argtable(int argc, char *argv[]){
     /* global arg_xxx structs */
    struct arg_lit *help;
    struct arg_end *end;

    void *argtable[] = {
        help    = arg_lit0("h", "help", "Display this help"),
        end     = arg_end(10),
    };

    arg_parse(argc,argv,argtable);
    if(help-> count > 0){
        console_log(NONE, "Usage: %s ", argv[0]);
        arg_print_syntax(stdout, argtable, "\r\n");
        console_log(NONE, "Directory operations\r\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\r\n");
        return 0;
    }else{
        diskOp.action = FORMAT;
        console_log(DEBUG, "Queue add command reset filesystem\r\n");
        xQueueSendToBack(s4436572_QueueFatfsCommand, &diskOp, TICK_COUNT);
        return 0;
    }
}