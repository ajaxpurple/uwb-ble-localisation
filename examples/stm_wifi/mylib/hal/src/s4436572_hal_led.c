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
#include "s4436572_hal_led.h"
#include "stm32l4xx_hal.h"
#include "s4436572_hal_board.h"

#define LED34    2

GPIO_TypeDef* LED_PORTS[NUM_LED_PINS] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED34_GPIO_PORT, LED5_GPIO_PORT};
uint16_t LED_PINS[NUM_LED_PINS] = {LED1_GPIO_PIN, LED2_GPIO_PIN, LED34_GPIO_PIN, LED5_GPIO_PIN};
uint8_t led34State = 0;

void s4436572_hal_led_init(){
    LED1_GPIO_PORT_CLK_ENABLE();
    LED2_GPIO_PORT_CLK_ENABLE();
    LED34_GPIO_PORT_CLK_ENABLE();
    LED5_GPIO_PORT_CLK_ENABLE();

    led34State = 0;
    GPIO_InitTypeDef  gpio_init_structure;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    for(int i=0; i<NUM_LED_PINS; i++){
        if(LED_PINS[i] != LED34_GPIO_PIN){
            gpio_init_structure.Pin   = LED_PINS[i];
            HAL_GPIO_Init(LED_PORTS[i], &gpio_init_structure);
        }
    }
    HAL_GPIO_WritePin(LED5_GPIO_PORT, LED5_GPIO_PIN, GPIO_PIN_SET);
}

void led_init(int led){
    GPIO_InitTypeDef  gpio_init_structure;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pin   = LED_PINS[led];
    HAL_GPIO_Init(LED_PORTS[led], &gpio_init_structure);
}

void led_deinit(int led){
    GPIO_InitTypeDef gpio_init_structure;    
    gpio_init_structure.Pin = LED_PINS[led];
    HAL_GPIO_WritePin(LED_PORTS[led], LED_PINS[led], GPIO_PIN_RESET);
    HAL_GPIO_DeInit(LED_PORTS[led], gpio_init_structure.Pin);
}

void s4436572_hal_led_toggle(int led){
    if(led != LED_WIFI && led != LED_BLE){
        if(led == LED_ERR){
            led--;
        }
        HAL_GPIO_TogglePin(LED_PORTS[led], LED_PINS[led]);
    }else{
        if(led34State){
            led_deinit(LED34);
        }else{
            led_init(LED34);
        }
        led34State ^= 1;
        if(led == LED_WIFI){
            HAL_GPIO_WritePin(LED_PORTS[LED34], LED_PINS[LED34], GPIO_PIN_SET);
        }else{
            HAL_GPIO_WritePin(LED_PORTS[LED34], LED_PINS[LED34], GPIO_PIN_RESET);
        }
    }
}

void s4436572_hal_led_write(int led, int value){
    if(led == 4){
        if((value & 1) == 1){
            HAL_GPIO_WritePin(LED_PORTS[led-1], LED_PINS[led-1], GPIO_PIN_RESET);
        }else{
            HAL_GPIO_WritePin(LED_PORTS[led-1], LED_PINS[led-1], GPIO_PIN_SET);
        }
    }
    if(led != 2 && led != 3){
        if((value & 1) == 1){
            HAL_GPIO_WritePin(LED_PORTS[led], LED_PINS[led], GPIO_PIN_SET);
        }else{
            HAL_GPIO_WritePin(LED_PORTS[led], LED_PINS[led], GPIO_PIN_RESET);
        }
    }else{
        if((value & 1) == 1){
            led_init(LED34);
        }else{
            led_deinit(LED34);
        }
        if(led == 2){
            HAL_GPIO_WritePin(LED_PORTS[LED34], LED_PINS[LED34], GPIO_PIN_SET);
        }else{
            HAL_GPIO_WritePin(LED_PORTS[LED34], LED_PINS[LED34], GPIO_PIN_RESET);
        }
    }
}


void s4436572_hal_led_deinit(){
    GPIO_InitTypeDef gpio_init_structure;    
    for(int i=0; i<NUM_LED_PINS; i++){
        gpio_init_structure.Pin = LED_PINS[i];
        HAL_GPIO_WritePin(LED_PORTS[i], LED_PINS[i], GPIO_PIN_RESET);
        HAL_GPIO_DeInit(LED_PORTS[i], gpio_init_structure.Pin);
    }
}