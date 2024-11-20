/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    October 2024
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_timer.h"
#include "stm32f4xx_rcc_mort.h"
#include "applications/sub_clock.h"
#include <cstdint>


/* MACRO definitions----------------------------------------------------------*/
// Timer Base Addresses
#define TIM3_BASE_ADDRESS 0x40000400
#define TIM4_BASE_ADDRESS 0x40000800
#define TIM5_BASE_ADDRESS 0x40000C00
#define TIM12_BASE_ADDRESS 0x40001800
#define TIM13_BASE_ADDRESS 0x40001C00
#define TIM14_BASE_ADDRESS 0x40002000

// Timer Register Offsets
#define SR_OFFSET 0x10
#define PSC_OFFSET 0x28
#define ARR_OFFSET 0x2C
#define CR1_OFFSET 0x00
#define CCMR1_OFFSET 0x18
#define CCMR2_OFFSET 0x1C
#define CCR1_OFFSET 0x34
#define CCR2_OFFSET 0x38
#define CCR3_OFFSET 0x3C
#define CCR4_OFFSET 0x40
#define CCER_OFFSET 0x20
#define CNT_OFFSET 0x24
#define DIER_OFFSET 0x0C

// Flags / Settings
#define TIM_UIEN        ((uint16_t)(0b1))       //update interrupt enable
#define TIM_UIF         ((uint16_t)(0b1))       //overflow interrupt flag


/* Function definitions----------------------------------------------------------*/
void initTimer (int timer_number, uint16_t psc, uint16_t arr)
{
    // Timer addresses
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);
    uint32_t * status_register = (uint32_t *)(long)(timer_base_address + SR_OFFSET);
    uint32_t * psc_register = (uint32_t *)(long)(timer_base_address + PSC_OFFSET);
    uint32_t * arr_register = (uint32_t *)(long)(timer_base_address + ARR_OFFSET);
    
    uint32_t * reg_pointer;

    // Enable APB1 clock
    enableAPB1RCCclock(timer_number);

    // Clear UIF flag in the SR
    reg_pointer = (uint32_t *)status_register;
    uint32_t current_status = *reg_pointer;
    *reg_pointer = ~current_status; //clear UIF flag without logical operations

    // Set prescaler value
    reg_pointer = (uint32_t *)psc_register;
    *reg_pointer = psc;

    // Set auto-reload register value
    reg_pointer = (uint32_t *)arr_register;
    *reg_pointer = arr;
}

void enableTimer(int timer_number)
{
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);

    // enable Timer
    uint16_t TIMCEN = 0b1;
    uint32_t * reg_pointer = (uint32_t *)(long)(timer_base_address + CR1_OFFSET);
    *reg_pointer = *reg_pointer | TIMCEN;
}

void initPWMonChannel(int timer_number, int channel_number, float duty_cycle)
{
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);
    uint32_t * arr_register = (uint32_t *)(long)(timer_base_address + ARR_OFFSET);
    uint32_t * ccmr1_register = (uint32_t *)(long)(timer_base_address + CCMR1_OFFSET);
    uint32_t * ccmr2_register = (uint32_t *)(long)(timer_base_address + CCMR2_OFFSET);
    uint32_t * ccr1_register = (uint32_t *)(long)(timer_base_address + CCR1_OFFSET);
    uint32_t * ccr2_register = (uint32_t *)(long)(timer_base_address + CCR2_OFFSET);
    uint32_t * ccr3_register = (uint32_t *)(long)(timer_base_address + CCR3_OFFSET);
    uint32_t * ccr4_register = (uint32_t *)(long)(timer_base_address + CCR4_OFFSET);
    uint32_t * ccer_register = (uint32_t *)(long)(timer_base_address + CCER_OFFSET);
    
    uint32_t * reg_pointer;

    uint32_t * ccmr_register;
    uint32_t * ccr_register;
    uint16_t OCxM_CLR;
    uint16_t OCxM_PWM;
    uint16_t CCxS_CLR;
    uint16_t OCxPE;
    switch (channel_number) {
        case 1 : {
                    ccmr_register = ccmr1_register; 
                    ccr_register = ccr1_register;
                    OCxM_CLR = ~((uint16_t) 0x70);
                    OCxM_PWM = 0x60;
                    CCxS_CLR = ~((uint16_t) 0b11);
                    OCxPE = 0b1000;
                    break;
                }
        case 2 : {
                    ccmr_register = ccmr1_register; 
                    ccr_register = ccr2_register;
                    OCxM_CLR = ~((uint16_t) 0x7000);
                    OCxM_PWM = 0x6000;
                    CCxS_CLR = ~((uint16_t) 0x300);
                    OCxPE = 0x400;
                    break;
                }
        case 3 : {
                    ccmr_register = ccmr2_register; 
                    ccr_register = ccr3_register;
                    OCxM_CLR = ~((uint16_t) 0x70);
                    OCxM_PWM = 0x60;
                    CCxS_CLR = ~((uint16_t) 0b11);
                    OCxPE = 0b1000;
                    break;
                }
        case 4 : {
                    ccmr_register = ccmr2_register; 
                    ccr_register = ccr4_register;
                    OCxM_CLR = ~((uint16_t) 0x7000);
                    OCxM_PWM = 0x6000;
                    CCxS_CLR = ~((uint16_t) 0x300);
                    OCxPE = 0x400;
                    break;
                }
        default : fprintf(stderr, "Invalid Channel Number\n");
    }

    reg_pointer = (uint32_t *)ccmr_register; // point to CCMR2

    *reg_pointer = *reg_pointer & OCxM_CLR; // clear bits in OCxM
    *reg_pointer = *reg_pointer | OCxM_PWM; //set OCxM to toggle

    // Set CHx to output by clearing CCxS
    *reg_pointer = *reg_pointer & CCxS_CLR;

    /* Set duty cycle counter */
    // Obtain ARR value
    reg_pointer = (uint32_t *)arr_register;
    uint16_t arr_value = *reg_pointer;
    
    // set duty cycle values for arr
    uint16_t CCRx_DUTY = arr_value*duty_cycle; // force cast into an unsigned int
    reg_pointer = (uint32_t *)ccr_register;
    *reg_pointer = (uint16_t) CCRx_DUTY;

    // Set preload enable OCxPE on CCMR
    reg_pointer = (uint32_t *)ccmr_register; // point to CCMR
    *reg_pointer = *reg_pointer | OCxPE;

    // enable Timer Channel x
    uint16_t CCXE_EN = 0b1<<(4*(channel_number-1)); 
    reg_pointer = (uint32_t *) ccer_register;
    *reg_pointer = *reg_pointer | CCXE_EN;
}

// enabling an interrupt based on the timer counting up to ARR
void initTimerIntOverflow(int timer_number)
{
    uint16_t *reg_pointer;
    
    // calc register location
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);
    uint16_t * dma_interrupt_enable_register = (uint16_t *)(long)(timer_base_address + DIER_OFFSET);
    
    // enable the nested vector interrupt
    enableNVIC_StdTimer(timer_number);
    // enable the update interrupt
    reg_pointer = dma_interrupt_enable_register;
    *reg_pointer = *reg_pointer | TIM_UIEN; // setting single bit, don't need to clear
}

/* *******************************************************************************
                    TIMER UPDATE FUNCTIONS
   ******************************************************************************* */

void setDutyCycle(int timer_number, int channel_number, float duty_cycle)
{
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);
    uint32_t * arr_register = (uint32_t *)(long)(timer_base_address + ARR_OFFSET);
    uint32_t * ccr1_register = (uint32_t *)(long)(timer_base_address + CCR1_OFFSET);
    uint32_t * ccr2_register = (uint32_t *)(long)(timer_base_address + CCR2_OFFSET);
    uint32_t * ccr3_register = (uint32_t *)(long)(timer_base_address + CCR3_OFFSET);
    uint32_t * ccr4_register = (uint32_t *)(long)(timer_base_address + CCR4_OFFSET);

    uint32_t * reg_pointer;

    uint32_t * ccr_register;
    switch (channel_number) {
        case 1 : {ccr_register = ccr1_register; break;}
        case 2 : {ccr_register = ccr2_register; break;}
        case 3 : {ccr_register = ccr3_register; break;}
        case 4 : {ccr_register = ccr4_register; break;}
        default : fprintf(stderr, "Received Invalid Channel Number at Set Duty cycle\n");
    }

    // Obtain ARR value
    reg_pointer = (uint32_t *)arr_register;
    uint16_t arr_value = *reg_pointer;
    
    // set duty cycle values for arr
    uint16_t CCRx_DUTY = arr_value*duty_cycle; // force cast into an unsigned int
    reg_pointer = (uint32_t *)ccr_register;
    *reg_pointer = (uint16_t) CCRx_DUTY;

}

/* *******************************************************************************
                    TIMER UTILITY FUNCTIONS
   ******************************************************************************* */
uint16_t getTIMCNT(int timer_number)
{
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(timer_number);
    uint32_t * reg_pointer = (uint32_t *)(long)(timer_base_address + CNT_OFFSET);

    // get TIM count and return
    return *reg_pointer;
}
   
uint32_t mapTimerNumberToBaseAddress(int timer_number)
{
    uint32_t timer_base_address;

    switch (timer_number) {
        case 3 : {timer_base_address = TIM3_BASE_ADDRESS; break;}
        case 4 : {timer_base_address = TIM4_BASE_ADDRESS; break;}
        case 5 : {timer_base_address = TIM5_BASE_ADDRESS; break;}
        case 12 : {timer_base_address = TIM12_BASE_ADDRESS; break;}
        case 13 : {timer_base_address = TIM13_BASE_ADDRESS; break;}
        case 14 : {timer_base_address = TIM14_BASE_ADDRESS; break;}
        default : fprintf(stderr, "Received Invalid Timer Number at Timer Base Address\n");
    }

    return timer_base_address;
}

void enableAPB1RCCclock(int timer_number)
{
    switch (timer_number) {
        case 3 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); break;}
        case 4 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); break;}
        case 5 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); break;}
        case 12 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); break;}
        case 13 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); break;}
        case 14 : {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); break;}
        default : fprintf(stderr, "Received Invalid Timer Number at APB1 clock enable\n");
    }
}

/* *******************************************************************************
                    TIMER INTERRUPT HANDLING
   ******************************************************************************* */

// service & clear the timer2 interrupt request
void TIM2_IRQHandler(void){

}

// service & clear the timer3 interrupt request
// Currently arranged for the timer3 overflow to be a running clock
void TIM3_IRQHandler(void){
    
    uint16_t *reg_pointer_sr;
    uint16_t *reg_pointer_dier;
    
    // address mapping
    uint32_t timer_base_address = mapTimerNumberToBaseAddress(3);
    uint16_t * status_register = (uint16_t *)(long)(timer_base_address + SR_OFFSET);
    uint16_t * dma_interrupt_enable_register = (uint16_t *)(long)(timer_base_address + DIER_OFFSET);

    reg_pointer_sr = (uint16_t *)status_register;
    reg_pointer_dier = (uint16_t *)dma_interrupt_enable_register;
    
    // verify that an overflow triggered the interrupt & that interrupts are enabled
    if(((*reg_pointer_sr & TIM_UIF) > 0) && ((*reg_pointer_dier & TIM_UIEN) > 0)) {
        // clear the interrupt
        *reg_pointer_sr = ~TIM_UIF;
        // iterate the wrap counter
        wrapTimer();
    }
}

// service & clear the timer4 interrupt request
void TIM4_IRQHandler(void){

}