/**
  ******************************************************************************
  * @file    hardware_stm_interrupt.c 
  * @author  stwade@andrew.cmu.edu
  * @version 1.0
  * @date    October-2024
  * @brief   Controls STM32F446ze Interrupts
  ******************************************************************************
  */

#include "hardware_stm_timer.h"
 #include "hardware_stm_interrupt.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>

#include "hardware_stm_gpio.h"

/* MACRO definitions----------------------------------------------------------*/
#define SYSTEM_CONTROL_BASE_ADDRESS (0xE000E000)
#define NVIC_BASE_ADDRESS (SYSTEM_CONTROL_BASE_ADDRESS + 0x100)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31 (NVIC_BASE_ADDRESS)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63 (NVIC_BASE_ADDRESS+0x4)
#define NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95 (NVIC_BASE_ADDRESS+0x8)
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31 (NVIC_BASE_ADDRESS + 0x80)
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_32_63 (NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31 + 0x4)
#define NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_64_95 (NVIC_INTERRUPT_CLEAR_ENABLE_REGISTER_0_31 + 0x8)
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31 (NVIC_BASE_ADDRESS + 0x100)
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_32_63 (NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31 + 0x4)
#define NVIC_INTERRUPT_SET_PENDING_REGISTER_64_95 (NVIC_INTERRUPT_SET_PENDING_REGISTER_0_31 + 0x8)
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 (NVIC_BASE_ADDRESS + 0x180)
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_32_63 (NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 + 0x4)
#define NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_64_95 (NVIC_INTERRUPT_CLEAR_PENDING_REGISTER_0_31 + 0x8)
#define TIM2_INTERRUPT_BIT (0x10000000)
#define TIM3_INTERRUPT_BIT (0x20000000)
#define TIM4_INTERRUPT_BIT (0x40000000)
#define EXTI9_5_INTERRUPT_BIT (0x800000)
#define SPI1_INTERRUPT_BIT (0x08)
#define SPI4_INTERRUPT_BIT (0x100000)
//For external interrupts:
#define SYSCFG_BASE_ADDRESS ((uint32_t)(0x40013800))
#define SYSCFG_EXTERNAL_INTERRUPT_REGISTER_2 (SYSCFG_BASE_ADDRESS + 0x0C)
#define SYSCFG_EXTERNAL_INTERRUPT_6_BITS ((uint32_t)0xF00) //flags for External interrupt register 2
#define SYSCFG_EXTERNAL_INTERRUPT_6_PORTC ((uint32_t)0x200)
//External interrupt controller :
#define EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS ((uint32_t)(0x40013C00))
#define EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER (EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS)
#define EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER_EXTI6 ((uint32_t)0x40) //flags for external interrupt controller mask register
#define EXTERNAL_INTERRUPT_CONTROLLER_RTSR (EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS+0x08)
#define EXTERNAL_INTERRUPT_CONTROLLER_RTSR_EXTI6 ((uint32_t)0x40)
#define EXTERNAL_INTERRUPT_CONTROLLER_FTSR (EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS+0x0C)
#define EXTERNAL_INTERRUPT_CONTROLLER_FTSR_EXTI6 ((uint32_t)0x40)
#define EXTERNAL_INTERRUPT_CONTROLLER_PENDING_REGISTER (EXTERNAL_INTERRUPT_CONTROLLER_BASE_ADDRESS+0x14)
#define EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6 ((uint32_t)0x40)

void enableSPI1Interrupt(void){
    uint32_t* reg_pointer_32;
    reg_pointer_32 = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_32_63;
    *reg_pointer_32 = SPI1_INTERRUPT_BIT;
}

void enableSPI4Interrupt(void){
    uint32_t* reg_pointer_32;
    reg_pointer_32 = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_64_95;
    *reg_pointer_32 = SPI4_INTERRUPT_BIT;
}

void enableNVIC_StdTimer(int timer_number)
{
    uint32_t * reg_pointer;
    
    // tim2 - tim4 in positions 28 - 30 of NVIC
    reg_pointer = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
    switch(timer_number) {
      case 2: *reg_pointer = TIM2_INTERRUPT_BIT; break;
      case 3: *reg_pointer = TIM3_INTERRUPT_BIT; break;
      case 4: *reg_pointer = TIM4_INTERRUPT_BIT; break;
      default: fprintf(stderr, "Tried to init NVIC with unsupported Timer ID"); break;
    }
}