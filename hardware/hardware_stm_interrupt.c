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
#include "inputs_remote.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>
#include "hardware_stm_gpio.h"
#include "applications/depth_sensor.h"
#include "timer_queue_remote.h"
#include "state_machine_remote.h"

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

static int interruptCounter = 0; // for debugging

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

void enableEXTI6OnPortC(void)
{
    // GPIO config
    initGPIOasMode(PORT_C, 6, MODE_IN, 0, PUPD_FLOAT, 0, 0); // setup PC6 as input, push pull, floating
    initGPIOasMode(PORT_B, 0, MODE_OUT, 0, PUPD_DOWN, 1, 0); // setup PB0 (internal LED) as an output, push pull, PD, init high (debug)
    
    uint32_t *reg_pointer_32;
    // to communicate / update SYSCFG address, need to enable APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // set the EXTI Control Register to GPIO of interest
    // in this case, set EXTI_CR2 EXTI6 bits to GPIO C (ie PC6)
    reg_pointer_32 = (uint32_t *)SYSCFG_EXTERNAL_INTERRUPT_REGISTER_2;
    *reg_pointer_32 = *reg_pointer_32 & (~SYSCFG_EXTERNAL_INTERRUPT_6_BITS);
    *reg_pointer_32 = *reg_pointer_32 | SYSCFG_EXTERNAL_INTERRUPT_6_PORTC;    
    // (EXTI config registers also on APB2, don't need to enable a separate bus)
    // Set mask register to un-mask EXTI6 (bit 6)
    reg_pointer_32 = (uint32_t *)EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER;
    *reg_pointer_32 = *reg_pointer_32 | EXTERNAL_INTERRUPT_CONTROLLER_MASK_REGISTER_EXTI6; // don't need to clear since this is 1 bit
    // EXTI6 trigger to falling edge
    reg_pointer_32 = (uint32_t *)EXTERNAL_INTERRUPT_CONTROLLER_FTSR;
    *reg_pointer_32 = *reg_pointer_32 | EXTERNAL_INTERRUPT_CONTROLLER_FTSR_EXTI6;
    // enable EXTI6 in the NVIC (bit 23)
    reg_pointer_32 = (uint32_t *)NVIC_INTERRUPT_SET_ENABLE_REGISTER_0_31;
    *reg_pointer_32 = EXTI9_5_INTERRUPT_BIT; // writing a 0 has no effect, so can hard set this
}    

// To allow some time to pass to check the state of the button 
void delay_button(uint32_t time)
{   
    //time is in ms 
    //10 = 10 ms
    //10000 = 10 s
    for (uint32_t i = 0; i < time ; i++)
    {
        //do nothing
    };
};

void EXTI9_5_IRQHandler(void)
{
    uint32_t *reg_pointer_32;
    reg_pointer_32 = (uint32_t *)EXTERNAL_INTERRUPT_CONTROLLER_PENDING_REGISTER;

    // check which interrupt fired
    // mask for if its EXTI6
    if((*reg_pointer_32 & EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6) > 0)
    {
        // clear pending interrupt (by writing a 1)
        *reg_pointer_32 = EXTERNAL_INTERRUPT_CONTROLLER_PENDING_EXTI6;

        uint32_t btn_val1 = read_land_button(); //initial state of the button
        delay_button(10); //delay for 10ms
        uint32_t btn_val2 = read_land_button(); //second check of button state

        if (btn_val1 == btn_val2 &&   // If the state of the button was the same before and after the delay 
            remote_state == DRIVE_REMOTE &&  // Must be in drive state
            sub_status_vals.target_detected) // AND if the beam is NOT broken
        {
            sched_event(LAND_REMOTE); // transition the remote into the landing state 
        }

        /**
        // Debugging code // 
        // toggle PB0 as our action (debugging the EXTI)
        ToggleGPIOOutput(PORT_B, PIN_0);

        //debugging depth calculation
        // iterate the interrupt counter
        interruptCounter = interruptCounter + 1;
        // every other button press, let's take a depth reading / talk to the sensor
        if(interruptCounter % 2 == 0) {
            printf("inches: %.2f \n", getDepth());
        } 
        // do a calculation and double check if things are making sense
        else {
            //getPressure();
        }        
        printf("pressure: %.2f \n", getPressure());
        */
    }
}
