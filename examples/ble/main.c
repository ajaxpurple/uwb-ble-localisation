#include "sdk_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp.h"
#include "boards.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_gpiote.h"
#include "nrf_uart.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf.h"

#include "SES/os_ble.h"
#include "app_timer.h"
#include "app_error.h"
#include "app_button.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "app_gpiote.h"
#include <string.h>
#include "port_platform.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"
#include "UART.h"
#include "queue.h"
#include "semphr.h"


#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

uint8_t node_id = 0;
bool is_synced = false;

/* Preamble timeout, in multiple of PAC size. See NOTE 3 below. */
#define PRE_TIMEOUT 1000

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name){
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

void reset(){
  is_synced = false;
  xQueueSendFromISR(bleSetId, &node_id, 0);
}

static void button_event_handler(uint8_t pin_no, uint8_t button_action){    
    if (button_action == APP_BUTTON_PUSH) {
        node_id++;
        node_id = node_id & 3;
        switch(node_id){
          case 0:
            LEDS_OFF(BSP_LED_2_MASK | BSP_LED_3_MASK);
            break;
          case 1:
            LEDS_OFF(BSP_LED_2_MASK);
            LEDS_ON(BSP_LED_3_MASK);
            break;
          case 2:
            LEDS_ON(BSP_LED_2_MASK);
            LEDS_OFF(BSP_LED_3_MASK);
            break;
          case 3:
            LEDS_ON(BSP_LED_2_MASK | BSP_LED_3_MASK);
            break;
        }
        
        reset();
    }    
}

void leds_setup(){
  LEDS_CONFIGURE(LEDS_MASK);
  LEDS_OFF(LEDS_MASK);
}

void button_setup(){
  static app_button_cfg_t buttons[] ={{2, false, BUTTON_PULL, button_event_handler}};
  APP_GPIOTE_INIT(1);
  app_button_init(buttons, 1, 10);
  app_button_enable();
}

void uwbTask(){
  for(;;){
    LEDS_INVERT(BSP_LED_0_MASK);
    vTaskDelay(200);
  }
}

void mainTask(){
  
  //xSemaphoreGive(bleBroadcastStart);
  
  uint16_t ping;
  uint8_t recv_node_id;
  bool ble_received[3] = {false};
  int8_t ble_rssi[3];
  int8_t rssi;
  bool all_done;
  for(;;){
    if(node_id > 0){
      if(!is_synced){
        xSemaphoreGive(bleScanStart);
        if(xQueueReceive(bleScanResult, &ping, 0) == pdPASS){
          LEDS_INVERT(BSP_LED_1_MASK);
          vTaskDelay(10);
          LEDS_INVERT(BSP_LED_1_MASK);
          
          recv_node_id = (ping>>8)&0xFF;
          rssi = (int8_t) (ping & 0xFF);
          printf("Node: %d, RSSI: %d\r\n", recv_node_id, rssi);
           if(recv_node_id == 0){
              printf("Synced from mobile node\r\n");
              xSemaphoreGive(bleBroadcastStart);
              LEDS_ON(BSP_LED_1_MASK);
              is_synced = true;
           }
        }
      }
   }else{
      if(!is_synced){
        xSemaphoreGive(bleBroadcastStart);
        xSemaphoreGive(bleBroadcastStop);
        xSemaphoreGive(bleScanStart);
        is_synced = true;
      }else{
        if(xQueueReceive(bleScanResult, &ping, 0) == pdPASS){
            LEDS_INVERT(BSP_LED_1_MASK);
            vTaskDelay(10);
            LEDS_INVERT(BSP_LED_1_MASK);
           recv_node_id = (ping>>8)&0xFF;
           rssi = (int8_t) (ping & 0xFF);
           if(recv_node_id > 0){
             printf("Node: %d, RSSI: %d\r\n", recv_node_id, rssi);
             ble_received[recv_node_id-1] = true;
             ble_rssi[recv_node_id-1] = rssi;
             all_done = true;
             for(uint8_t i=0; i<3; i++){
              all_done = all_done && ble_received[i];
             }
             if(all_done){
                printf("ble: [%d, %d, %d]\r\n", ble_rssi[0], ble_rssi[1], ble_rssi[2]);
                for(uint8_t i=0; i<3; i++){
                  ble_received[i] = false;
                }
             }
           }
        }
      }
   }
   vTaskDelay(100);
 }
}


int main(void){  
  leds_setup();
  button_setup();
  ble_setup();
  boUART_Init();  
  
  xTaskCreate( bleTask, "ble", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  xTaskCreate( uwbTask, "uwb", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  xTaskCreate( mainTask, "main", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  vTaskStartScheduler();
  while(1){
    APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
  }
}