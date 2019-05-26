#include "s4436572_hal_mouse.h"
#include "s4436572_hal_board.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

struct mouseHID_t
{
uint8_t buttons ;
int8_t x ;
int8_t y ;
int8_t wheel ;
};



USBD_HandleTypeDef hUsbDeviceFS;
extern USBD_DescriptorsTypeDef FS_Desc;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
struct mouseHID_t mouseHID;

void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

void HAL_GPIO_EXTI_Callback(uint16_t gpioPin){
    if(gpioPin == GPIO_PIN_13){
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
            s4436572_hal_mouse_down();
        }else{
            s4436572_hal_mouse_up();
        }
    }
}

void usb_init(){
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID);
    USBD_Start(&hUsbDeviceFS);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_OTG_FS_PWR_EN_GPIO_Port, USB_OTG_FS_PWR_EN_Pin, GPIO_PIN_RESET);


  /*Configure GPIO pins : USB_OTG_FS_OVRCR_EXTI3_Pin SPSGRF_915_GPIO3_EXTI5_Pin SPBTLE_RF_IRQ_EXTI6_Pin ISM43362_DRDY_EXTI1_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_OVRCR_EXTI3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OTG_FS_OVRCR_EXTI3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_OTG_FS_PWR_EN_Pin SPBTLE_RF_SPI3_CSN_Pin PMOD_RESET_Pin STSAFE_A100_RESET_Pin */
  GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_OTG_FS_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void s4436572_hal_mouse_init(){
    MX_GPIO_Init();
    usb_init();
    mouseHID.buttons = 0;
    mouseHID.wheel = 0;
}

void s4436572_hal_mouse_move(int8_t dx, int8_t dy){
    mouseHID.x = dx;
    mouseHID.y = dy;
    USBD_HID_SendReport (&hUsbDeviceFS , (uint8_t*) &mouseHID , sizeof ( struct mouseHID_t ));
}

void s4436572_hal_mouse_down(){
    mouseHID.buttons = 1;
    mouseHID.x = 0;
    mouseHID.y = 0;
    USBD_HID_SendReport (&hUsbDeviceFS , (uint8_t*) &mouseHID , sizeof ( struct mouseHID_t ));
}
void s4436572_hal_mouse_up(){
    mouseHID.buttons = 0;
    mouseHID.x = 0;
    mouseHID.y = 0;
    USBD_HID_SendReport (&hUsbDeviceFS , (uint8_t*) &mouseHID , sizeof ( struct mouseHID_t ));
}