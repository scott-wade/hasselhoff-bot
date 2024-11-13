/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    October 2024
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>


/* MACRO definitions----------------------------------------------------------*/
#define TIM3_BASE_ADDRESS 0x40000400
#define TIM3_SR (TIM3_BASE_ADDRESS + 0x10)
#define TIM3_PSC (TIM3_BASE_ADDRESS + 0x28)
#define TIM3_ARR (TIM3_BASE_ADDRESS + 0x2C)
#define TIM3_CR1 (TIM3_BASE_ADDRESS + 0x00)
#define TIM3_CCMR2 (TIM3_BASE_ADDRESS + 0x1C)
#define TIM3_CCR3 (TIM3_BASE_ADDRESS + 0x3C)
#define TIM3_CCER (TIM3_BASE_ADDRESS + 0x20)
#define TIM3_CNT (TIM3_BASE_ADDRESS + 0x24)


/* Function definitions----------------------------------------------------------*/
void initTim3 (uint16_t psc, uint16_t arr)
{
    uint32_t * reg_pointer;

    // Enable APB1 clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // Clear UIF flag in TIM3_SR
    reg_pointer = (uint32_t *)TIM3_SR;
    uint32_t current_status = *reg_pointer;
    *reg_pointer = ~current_status; //clear UIF flag without logical operations

    // Set prescaler value
    reg_pointer = (uint32_t *)TIM3_PSC;
    *reg_pointer = psc;

    // Set auto-reload register value
    reg_pointer = (uint32_t *)TIM3_ARR;
    *reg_pointer = arr;
    
    // enable Timer 3
    uint16_t TIM3CEN = 0b1;
    reg_pointer = (uint32_t *)TIM3_CR1;
    *reg_pointer = *reg_pointer | TIM3CEN;
}

uint16_t getTIM3CNT(void)
{
    uint32_t * reg_pointer = (uint32_t *)TIM3_CNT;

    // get TIM3 count and return
    return *reg_pointer;
}

void resetTIM3(void)
{
    uint32_t * reg_pointer = (uint32_t *)TIM3_CNT;

    uint16_t TIM3RESET = 0;
    *reg_pointer = TIM3RESET;
}