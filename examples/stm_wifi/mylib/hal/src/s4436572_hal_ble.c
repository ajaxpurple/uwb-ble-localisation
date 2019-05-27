/**
 ******************************************************************************
 * @file    mylib/s4436572_hal_ble.c
 * @author  Rohan Malik - 44365721
 * @date    29032019
 * @brief   BlueNRG BLE HAL library
 *
 ******************************************************************************
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 * s4436572_hal_ble_init() - Setup Bluetooth
 * s4436572_hal_ble_deinit() - Deinitialise Bluetooth
 * s4436572_hal_ble_write(message) - Write to Bluetooth
 * s4436572_hal_ble_read(message, length) - Read from Bluetooth
 ******************************************************************************
 */
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx.h"
#include "s4436572_hal_ble.h"
#include "s4436572_hal_board.h"
#include "bluenrg_gap.h"
#include "hci.h"
#include "hci_const.h"

#include "hci_le.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_utils.h"
#include "hci_tl.h"

void user_notify(void * pData);
void default_callback(Advertisement adv);

#define BDADDR_SIZE 6

enum States{
  TX, RX, IDLE
};

/* Private variables ---------------------------------------------------------*/
uint8_t state = IDLE;


const char name[] = "S4436572";
uint8_t manufacturerData[] = {
  26,
  AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
  0x4c, 0x00, 
  0x02, 
  0x15, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0xB9
};

uint8_t flags[] = {
  2,
  AD_TYPE_FLAGS,
  (FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED)
};

uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

extern EXTI_HandleTypeDef hexti6;
Advertisement advertisement;

void (*rxCallback)(Advertisement adv) = &default_callback;

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  BSP_SPI3_ReInit();
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_EXTI_IRQHandler(&hexti6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void ble_gpio_init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = BLE_EXTI_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLE_EXTI_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BLE_RST_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_RST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BLE_CSN_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_CSN_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = WIFI_BLE_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_BLE_GPIO_PORT, &GPIO_InitStruct);


  HAL_GPIO_WritePin(BLE_RST_GPIO_PORT, BLE_RST_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BLE_CSN_GPIO_PORT, BLE_CSN_GPIO_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(WIFI_BLE_GPIO_PORT, WIFI_BLE_GPIO_PIN, GPIO_PIN_RESET);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void s4436572_hal_ble_init(uint8_t* serverAddress){    
  ble_gpio_init();
  hci_init(&user_notify, NULL);
  hci_reset();
  HAL_Delay(100);
  aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, serverAddress);
  aci_gatt_init();  
  aci_gap_init_IDB05A1(GAP_OBSERVER_ROLE_IDB05A1 | GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x08, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  aci_hal_set_tx_power_level(1,4);
  hci_le_set_scan_resp_data(0, NULL);
}

void s4436572_hal_ble_check(){
  BSP_SPI3_ReInit();
  hci_user_evt_proc();
}

void s4436572_hal_ble_scan_start(void (*callback)(Advertisement adv)){
  rxCallback = callback;
  if(state!=RX){
    if(state == TX){
      s4436572_hal_ble_advertise_stop();
    }
    BSP_SPI3_ReInit();
    aci_gap_start_observation_procedure(20, 20, PASSIVE_SCAN, PUBLIC_ADDR, 0x00); 
  }
  state = RX;
}

void s4436572_hal_ble_scan_stop(){
  if(state == RX){
    BSP_SPI3_ReInit();
    aci_gap_terminate_gap_procedure(GAP_OBSERVATION_PROC_IDB05A1);
  }
  state = IDLE;
}

void s4436572_hal_ble_set_uuid(uint8_t* uuid){
  for(uint8_t i=0; i<16; i++){
    manufacturerData[6 + i] = uuid[i];
  }
}

void s4436572_hal_ble_advertise_start(){
  if(state == RX){
    s4436572_hal_ble_scan_stop();
  }
  BSP_SPI3_ReInit();
  aci_gap_set_discoverable(ADV_SCAN_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE, 0, NULL, 0, NULL, 0, 0);  
  aci_gap_delete_ad_type(AD_TYPE_TX_POWER_LEVEL);    
  state = TX;
}

void s4436572_hal_ble_advertise_stop(){
  BSP_SPI3_ReInit();
  aci_gap_set_non_discoverable();
  state = IDLE;
}

tBleStatus s4436572_hal_ble_advertise(uint16_t major, uint16_t minor){  
  if(state != TX){
    s4436572_hal_ble_advertise_start();
  }
  manufacturerData[22] = (major>>8) & 0xFF;
  manufacturerData[23] =  major & 0xFF;
  manufacturerData[24] = (minor>>8) & 0xFF;
  manufacturerData[25] =  minor & 0xFF;
  BSP_SPI3_ReInit();
  aci_gap_update_adv_data(sizeof(flags), flags);
  return aci_gap_update_adv_data(sizeof(manufacturerData), manufacturerData);
}

int extract_ibeacon_data(Advertisement advertisement, IBeaconData* beaconData){
  uint8_t subLength;
  uint8_t* subData;
  uint8_t status = 0;
  uint8_t length = advertisement.length;
  uint8_t* data = advertisement.data;
  beaconData->rssi = advertisement.rssi;
  while(length != 0){
    subData = data;
    subLength = subData[0];
    //subType = subData[1];
    subData += 2;
    if(subData[0] == 0x4c && subData[1] == 0x00 && subData[2] == 0x02 && subData[3] == 0x15){
      for(uint8_t i=0; i<16; i++){
        beaconData->uuid[i] = subData[i + 4];
      }
      subData += 20;
      beaconData->major = (subData[0] << 8) | subData[1];
      beaconData->minor = (subData[2] << 8) | subData[3];
      beaconData->rssiAt1m = (int8_t) subData[4];
      status = 1;
    }
    length -= subLength + 1;
    data += subLength + 1;
  }
  return status;
}

void default_callback(Advertisement adv_evt){
  //debug_printf("Advertisement from %02x:%02x:%02x:%02x:%02x:%02x with %d bytes of data: ", 
  //  adv_evt.address[0], adv_evt.address[1], adv_evt.address[2], adv_evt.address[3], adv_evt.address[4], adv_evt.address[5], adv_evt.length);
  //for(int i=0; i<adv_evt.length; i++){
  //  debug_printf("%02x ", adv_evt.data[i]);
  //}
  //debug_printf("\r\n");
}

void user_notify(void * pData)
{
  hci_uart_pckt *hci_pckt = pData;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  evt_le_meta_event *evt;
  evt_le_connection_complete *cc;
  le_advertising_info* adv_evt;
  
  if(hci_pckt->type != HCI_EVENT_PKT){
    //debug_printf("Type: %d\r\n", hci_pckt->type);
  }
  switch(event_pckt->evt){
    case EVT_DISCONN_COMPLETE:
      debug_printf("Disconnection complete\r\n");
      break;

    case EVT_LE_META_EVENT:
      evt = (void *)event_pckt->data;
      switch(evt->subevent){
        
        case EVT_LE_CONN_COMPLETE:
          cc = (void *)evt->data;
          debug_printf("Connected to %d, %d\r\n", cc->peer_bdaddr, cc->handle);
          break;

        case EVT_LE_ADVERTISING_REPORT:
          adv_evt = (void*)(evt->data + 1);
          
          for(uint8_t i=0; i<6; i++){
            advertisement.address[i] = adv_evt->bdaddr[i];
          }
          advertisement.length = adv_evt->data_length;
          for(uint8_t i=0; i<adv_evt->data_length; i++){
            advertisement.data[i] = adv_evt->data_RSSI[i];
          }
          advertisement.rssi = (int8_t) adv_evt->data_RSSI[adv_evt->data_length];
          (*rxCallback)(advertisement);
          break;
      }
      break;
  }
}