#include "stm32l4xx_hal.h"
#include "s4436572_hal_board.h"

#include "stm32l4xx.h"
#include "s4436572_hal_printf.h"
#include "s4436572_hal_ultrasonic.h"
#include "system_stm32l4xx.h"
#include <math.h>


void s4436572_hal_ultrasonic_init() {
    
    ULTRASONIC_GPIO_PORT_CLK_ENABLE();
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.Pin = ULTRASONIC_TRIG_GPIO_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ULTRASONIC_GPIO_PORT, &gpio_init_structure);

    /* Configure USART Rx as alternate function */
    gpio_init_structure.Pin = ULTRASONIC_ECHO_GPIO_PIN;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ULTRASONIC_GPIO_PORT, &gpio_init_structure);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;

}

void s4436572_hal_ultrasonic_deinit() {
    HAL_GPIO_DeInit(ULTRASONIC_GPIO_PORT, ULTRASONIC_ECHO_GPIO_PIN);
    HAL_GPIO_DeInit(ULTRASONIC_GPIO_PORT, ULTRASONIC_TRIG_GPIO_PIN);
}

/**
 * @brief Delay for a certain number of microseconds
 * @param delay - The number of microseconds to delay
 */
void delay_us(uint32_t delay) {

	//Get number of clock cycles to delay
	volatile uint32_t cycles = (SystemCoreClock / 1000000L) * delay;

	//Wait until the cycle count passes a certain number of clock cycles
	DWT->CYCCNT = 0;
	do {
	} while (DWT->CYCCNT < cycles);
}

uint16_t s4436572_hal_ultrasonic_read() {
    HAL_GPIO_WritePin(ULTRASONIC_GPIO_PORT, ULTRASONIC_TRIG_GPIO_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(ULTRASONIC_GPIO_PORT, ULTRASONIC_TRIG_GPIO_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(ULTRASONIC_GPIO_PORT, ULTRASONIC_TRIG_GPIO_PIN, GPIO_PIN_RESET);
    while(HAL_GPIO_ReadPin(ULTRASONIC_GPIO_PORT, ULTRASONIC_ECHO_GPIO_PIN) == GPIO_PIN_RESET){

    }
    uint32_t duration = 0;
    while((ULTRASONIC_GPIO_PORT->IDR & ULTRASONIC_ECHO_GPIO_PIN) != 0x00u){
        duration++;
        delay_us(40);
    }
    if(duration < 1000){
        return duration;
    }else{
        return 0xFFFF;
    }
}