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
#include "ss_resp_main.h"

#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */
#define POLL_TX_TO_RESP_RX_DLY_UUS      100

uint8_t node_id = 1;

TaskHandle_t  ss_responder_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void ss_responder_task_function (void * pvParameter);

static dwt_config_t config = {
  5,                /* Channel number. */
  DWT_PRF_64M,      /* Pulse repetition frequency. */
  DWT_PLEN_128,     /* Preamble length. Used in TX only. */
  DWT_PAC8,         /* Preamble acquisition chunk size. Used in RX only. */
  10,               /* TX preamble code. Used in TX only. */
  10,               /* RX preamble code. Used in RX only. */
  0,                /* 0 to use standard SFD, 1 to use non-standard SFD. */
  DWT_BR_6M8,       /* Data rate. */
  DWT_PHRMODE_STD,  /* PHY header mode. */
  (129 + 8 - 8)     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};


/* Preamble timeout, in multiple of PAC size. See NOTE 3 below. */
#define PRE_TIMEOUT 1000

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name){
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

void reset(){
    
  switch(node_id){
    
    case 0:           //Mobile
      LEDS_OFF(BSP_LED_2_MASK | BSP_LED_3_MASK);
      break;

    case 1:           //Static Node 1
      LEDS_OFF(BSP_LED_2_MASK);
      LEDS_ON(BSP_LED_3_MASK);
      static_node_change_id(1);
      break;
                
    case 2:           //Static Node 2
      
      LEDS_ON(BSP_LED_2_MASK);
      LEDS_OFF(BSP_LED_3_MASK);
      static_node_change_id(2);
      break;

    case 3:           //Static Node 3
      LEDS_ON(BSP_LED_2_MASK | BSP_LED_3_MASK);
      static_node_change_id(3);
      break;

  }
  xQueueSendFromISR(bleSetId, &node_id, 0);
}

static void button_event_handler(uint8_t pin_no, uint8_t button_action){    
    
    if (button_action == APP_BUTTON_PUSH) {
        node_id = (node_id % 3) + 1;
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
  uint16_t ping;
  uint8_t recv_node_id;
  bool ble_received[3] = {false};
  int8_t ble_rssi[3];
  int8_t rssi;
  bool all_done;
  xSemaphoreGive(bleScanStart);
  xSemaphoreGive(bleBroadcastStart);
  reset();
  for(;;){
        if(xQueueReceive(bleScanResult, &ping, 0) == pdPASS){
          LEDS_INVERT(BSP_LED_1_MASK);
          vTaskDelay(10);
          LEDS_INVERT(BSP_LED_1_MASK);
          recv_node_id = (ping>>8)&0xFF;
          rssi = (int8_t) (ping & 0xFF);
          printf("Node: %d, RSSI: %d\r\n", recv_node_id, rssi);
           if(recv_node_id == 0){
              printf("Synced from mobile node\r\n");
              //xSemaphoreGive(bleBroadcastStop);
              //xSemaphoreGive(bleBroadcastStart);
              reset();
              LEDS_ON(BSP_LED_1_MASK);
           }
        }
   vTaskDelay(100);
  }
}


void uwb_recv_setup(void) {
  
  UNUSED_VARIABLE(xTaskCreate(ss_responder_task_function, "SSTWR_RESP", configMINIMAL_STACK_SIZE + 200, NULL, tskIDLE_PRIORITY + 1, &ss_responder_task_handle)); 

  //-------------dw1000  ini------------------------------------	

  /* Setup DW1000 IRQ pin */
  nrf_gpio_cfg_input(DW1000_IRQ, NRF_GPIO_PIN_NOPULL); 		//irq

  /* Reset DW1000 */
  reset_DW1000(); 

  /* Set SPI clock to 2MHz */
  port_set_dw1000_slowrate();			

  /* Init the DW1000 */
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
  {
    //Init of DW1000 Failed
    while (1)
    {};
  }

  // Set SPI to 8MHz clock
  port_set_dw1000_fastrate();  

  /* Configure DW1000. */
  dwt_configure(&config);

  /* Apply default antenna delay value. Defined in port platform.h */
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  /* Set preamble timeout for expected frames.  */
  //dwt_setpreambledetecttimeout(PRE_TIMEOUT);

  dwt_setrxtimeout(0);    // set to NO receive timeout   
}

int main(void){  
  leds_setup();
  button_setup();
  ble_setup();
  uwb_recv_setup();
  boUART_Init();  
  
  xTaskCreate( bleTask, "ble", 100, NULL, tskIDLE_PRIORITY + 2, NULL );
  xTaskCreate( mainTask, "main", 100, NULL, tskIDLE_PRIORITY + 2, NULL );
  xTaskCreate( uwbTask, "uwb", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  vTaskStartScheduler();
  while(1){
    APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
  }
}