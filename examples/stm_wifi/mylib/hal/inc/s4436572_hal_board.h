/**
 ******************************************************************************
 * @file    mylib/inc/stm32l475_board.h
 * @author  Rohan Malik - 44365721
 * @date    18032019
 * @brief   Board config
 ******************************************************************************
 */
#ifndef BOARD_H
#define BOARD_H
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_gpio_ex.h"

#define PMOD_UART

#ifdef STLINK_UART
 #define UART                        USART1
 #define UART_GPIO_PORT              GPIOB
 #define UART_TX_GPIO_PIN            GPIO_PIN_6
 #define UART_RX_GPIO_PIN            GPIO_PIN_7
 #define UART_AF                     GPIO_AF7_USART1
 #define UART_GPIO_PORT_CLK_ENABLE   __HAL_RCC_GPIOB_CLK_ENABLE
 #define UART_GPIO_PORT_CLK_DISABLE  __HAL_RCC_GPIOB_CLK_DISABLE
 #define UART_CLK_ENABLE             __HAL_RCC_USART1_CLK_ENABLE
 #define UART_CLK_DISABLE            __HAL_RCC_USART1_CLK_DISABLE
 #define UART_IRQ_HANDLER            USART1_IRQHandler
 #define UART_IRQ                    USART1_IRQn
#endif

#ifdef PMOD_UART
    #define UART                        USART2
    #define UART_GPIO_PORT              GPIOD
    #define UART_TX_GPIO_PIN            GPIO_PIN_5
    #define UART_RX_GPIO_PIN            GPIO_PIN_6
    #define UART_AF                     GPIO_AF7_USART2
    #define UART_GPIO_PORT_CLK_ENABLE   __HAL_RCC_GPIOD_CLK_ENABLE
    #define UART_GPIO_PORT_CLK_DISABLE  __HAL_RCC_GPIOD_CLK_DISABLE
    #define UART_CLK_ENABLE             __HAL_RCC_USART2_CLK_ENABLE
    #define UART_CLK_DISABLE            __HAL_RCC_USART2_CLK_DISABLE
    #define UART_IRQ_HANDLER            USART2_IRQHandler
    #define UART_IRQ                    USART2_IRQn
#endif

#define NUM_LED_PINS                     4

#define LED1_GPIO_PORT               GPIOA
#define LED1_GPIO_PIN                GPIO_PIN_5
#define LED1_GPIO_PORT_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE
#define LED1_GPIO_PORT_CLK_DISABLE   __HAL_RCC_GPIOA_CLK_DISABLE

#define LED2_GPIO_PORT               GPIOB
#define LED2_GPIO_PIN                GPIO_PIN_14
#define LED2_GPIO_PORT_CLK_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE
#define LED2_GPIO_PORT_CLK_DISABLE   __HAL_RCC_GPIOB_CLK_DISABLE

#define LED34_GPIO_PORT               GPIOC
#define LED34_GPIO_PIN                GPIO_PIN_9
#define LED34_GPIO_PORT_CLK_ENABLE    __HAL_RCC_GPIOC_CLK_ENABLE
#define LED34_GPIO_PORT_CLK_DISABLE   __HAL_RCC_GPIOC_CLK_DISABLE

#define LED5_GPIO_PORT               GPIOE
#define LED5_GPIO_PIN                GPIO_PIN_3
#define LED5_GPIO_PORT_CLK_ENABLE    __HAL_RCC_GPIOE_CLK_ENABLE
#define LED5_GPIO_PORT_CLK_DISABLE   __HAL_RCC_GPIOE_CLK_DISABLE

#define BLE_CSN_GPIO_PIN        GPIO_PIN_13
#define BLE_CSN_GPIO_PORT       GPIOD
#define BLE_RST_GPIO_PIN        GPIO_PIN_8
#define BLE_RST_GPIO_PORT       GPIOA
#define BLE_EXTI_GPIO_PIN       GPIO_PIN_6
#define BLE_EXTI_GPIO_PORT      GPIOE
#define BLE_EXTI_IRQn           EXTI9_5_IRQn
#define WIFI_BLE_GPIO_PIN               LED34_GPIO_PIN
#define WIFI_BLE_GPIO_PORT              LED34_GPIO_PORT

#define ULTRASONIC_GPIO_PORT            GPIOA
#define ULTRASONIC_TRIG_GPIO_PIN        GPIO_PIN_4
#define ULTRASONIC_ECHO_GPIO_PIN        GPIO_PIN_6
#define ULTRASONIC_GPIO_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE 

#define USB_OTG_FS_OVRCR_EXTI3_Pin          GPIO_PIN_3
#define USB_OTG_FS_OVRCR_EXTI3_GPIO_Port    GPIOE
#define USB_OTG_FS_PWR_EN_Pin               GPIO_PIN_12
#define USB_OTG_FS_PWR_EN_GPIO_Port         GPIOD

#endif