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

#include "../common/os_ble.h"
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
#include "ss_init_main.h"

#define DEBUG 1
#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */
#define POLL_TX_TO_RESP_RX_DLY_UUS      100

uint8_t node_id = 0;
bool is_synced = false;
int8_t ble_rssi[5];

TaskHandle_t  ss_initiator_task_handle;   /**< Reference to TWR Initiator FreeRTOS task. */
extern void ss_initiator_task_function (void * pvParameter);

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

static void button_event_handler(uint8_t pin_no, uint8_t button_action){    
    if (button_action == APP_BUTTON_PUSH) {
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

void printTask(){
  for(;;){
    printf("{\"uwb\":[%d, %d, %d],", val1, val2, val3);
    printf("\"ble\":[%d, %d, %d, %d, %d]}\r",  ble_rssi[0], ble_rssi[1], ble_rssi[2], ble_rssi[3], ble_rssi[4]);
    for(uint8_t i=0; i<5; i++){
      ble_rssi[i] = 127;
    }
    vTaskDelay(500);
  }
}

void mainTask(){
  
  uint16_t ping;
  uint8_t recv_node_id;
  
  //bool ble_received[5] = {false};
  

  int8_t rssi;
  
  bool all_done;
  
  xSemaphoreGive(bleBroadcastStart);
  xSemaphoreGive(bleScanStart);
  
  int task_counter = 0;

  for(;;){
      
      if (xQueueReceive(bleScanResult, &ping, 50) == pdPASS) {
         
         LEDS_INVERT(BSP_LED_1_MASK);
         vTaskDelay(10);
         LEDS_INVERT(BSP_LED_1_MASK);

         recv_node_id = (ping>>8)&0xFF;
         rssi = (int8_t) (ping & 0xFF);
         
         if(recv_node_id > 0){
             //printf("Node: %d, RSSI: %d\r\n", recv_node_id, rssi);
             //printf("USS: %d\n\r", rssi);
             //ble_received[recv_node_id-1] = true;
             ble_rssi[recv_node_id-1] = rssi;
             //all_done = true;
             //for(uint8_t i=0; i<3; i++){
             // all_done = all_done && ble_received[i];
             //}
           //if(all_done && (xTaskGetTickCount() - task_counter > 500)){
           //   printf("{\"ble\":[%d,%d,%d, %d, %d]}\r", ble_rssi[0], ble_rssi[1], ble_rssi[2], ble_rssi[3], ble_rssi[4]);
            //  for(uint8_t i=0; i<3; i++){
            //    ble_received[i] = false;
            //  }

            //  task_counter = xTaskGetTickCount();
           //}
        }
      }
 }
}


void uwb_setup(void) {
  
  UNUSED_VARIABLE(xTaskCreate(ss_initiator_task_function, "SSTWR_INIT", configMINIMAL_STACK_SIZE + 200, NULL, tskIDLE_PRIORITY + 1, &ss_initiator_task_handle));

/* Setup DW1000 IRQ pin */  
  nrf_gpio_cfg_input(DW1000_IRQ, NRF_GPIO_PIN_NOPULL); 		//irq
  
  /*Initialization UART*/
  boUART_Init ();
  //printf("Singled Sided Two Way Ranging Initiator Example \r\n");
  
  /* Reset DW1000 */
  reset_DW1000(); 

  /* Set SPI clock to 2MHz */
  port_set_dw1000_slowrate();			
  
  /* Init the DW1000 */
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
  {
    //Init of DW1000 Failed
    while (1) {};
  }

  // Set SPI to 8MHz clock
  port_set_dw1000_fastrate();

  /* Configure DW1000. */
  dwt_configure(&config);

  /* Apply default antenna delay value. See NOTE 2 below. */
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  /* Set preamble timeout for expected frames. See NOTE 3 below. */
  //dwt_setpreambledetecttimeout(0); // PRE_TIMEOUT
          
  /* Set expected response's delay and timeout. 
  * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(65000); // Maximum value timeout with DW1000 is 65ms  
}

int main(void){  
  leds_setup();
  ble_setup();
  uwb_setup();
  boUART_Init();  
  
  xTaskCreate( bleTask, "ble", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  xTaskCreate( uwbTask, "uwb", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  xTaskCreate( mainTask, "main", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  xTaskCreate( printTask, "print", 100, NULL, tskIDLE_PRIORITY + 1, NULL );
  vTaskStartScheduler();
  while(1){
    APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
  }
}