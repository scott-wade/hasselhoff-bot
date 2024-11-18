/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    November 2024
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_gpio.h"
#include "stdio.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>


/* MACRO definitions----------------------------------------------------------*/
//GPIO addresses
#define PORTA_BASE_ADDRESS ((uint32_t) 0x40020000)
#define PORTB_BASE_ADDRESS ((uint32_t) 0x40020400)
#define PORTC_BASE_ADDRESS ((uint32_t) 0x40020800)
#define PORTD_BASE_ADDRESS ((uint32_t) 0x40020C00)
#define PORTE_BASE_ADDRESS ((uint32_t) 0x40021000)
#define PORTF_BASE_ADDRESS ((uint32_t) 0x40021400)
#define PORTG_BASE_ADDRESS ((uint32_t) 0x40021800)
#define PORTH_BASE_ADDRESS ((uint32_t) 0x40021C00)

// Register Offsets
#define MODER_REGISTER_OFFSET 0x00
#define OTYPER_REGISTER_OFFSET 0x04
#define OSPEEDR_REGISTER_OFFSET 0x08
#define PUPDR_REGISTER_OFFSET 0x0C
#define ODR_REGISTER_OFFSET 0x14
#define IDR_REGISTER_OFFSET 0x10
#define AFRL_REGISTER_OFFSET 0x20
#define AFRH_REGISTER_OFFSET 0x24


/* function definitions----------------------------------------------------------*/

/* *******************************************************************************
                    GPIO INITIALIZATION
    mapping port number: 0->A, 1->B, ... 7->H
    mode: 0-input, 1-output, 2-AF
    open_drain: 0-pupd, 1-open drain
    pupd: 0-floating, 1-pull up, 2-pull down
    init_output: [0,1] initial value if in output mode 
    alt_func: number of alternate function mode
   ******************************************************************************* */
void initGPIOasMode(uint8_t port_number, uint8_t pin_number, uint8_t mode, uint8_t open_drain, uint8_t pupd, uint8_t init_output, uint8_t alt_func)
{
    uint32_t port_base_address = mapPortNumbertoBaseAddress(port_number);
    uint32_t * moder_register = (uint32_t *)(long)(port_base_address + MODER_REGISTER_OFFSET);
    uint32_t * otyper_register = (uint32_t *)(long)(port_base_address + OTYPER_REGISTER_OFFSET);
    uint32_t * ospeedr_register = (uint32_t *)(long)(port_base_address + OSPEEDR_REGISTER_OFFSET);
    uint32_t * pupdr_register = (uint32_t *)(long)(port_base_address + PUPDR_REGISTER_OFFSET);
    uint32_t * odr_register = (uint32_t *)(long)(port_base_address + ODR_REGISTER_OFFSET);
    uint32_t * afrl_register = (uint32_t *)(long)(port_base_address + AFRL_REGISTER_OFFSET);
    uint32_t * afrh_register = (uint32_t *)(long)(port_base_address + AFRH_REGISTER_OFFSET);
    
    uint32_t * reg_pointer;

    // Enable Peripheral Clock (All GPIOs are on AHB1)
    enableAHB1GPIOclock(port_number);

    /* Pin configuration */
    if (mode == MODE_IN) { // Input

        uint32_t MODER_IN = ~((uint32_t) ((1<<(2*pin_number+1)) + (1<<(2*pin_number))));
        reg_pointer = (uint32_t *)moder_register;
        *reg_pointer = *reg_pointer & MODER_IN;

    } else if (mode == MODE_OUT) { // Output

        uint32_t MODER_CLR = ~((uint32_t) (0b11 << (2*pin_number)));
        uint32_t MODER_OUT = 1<<(2*pin_number);
        reg_pointer = (uint32_t *)moder_register;
        *reg_pointer = *reg_pointer & MODER_CLR;
        *reg_pointer = *reg_pointer | MODER_OUT;

    } else if (mode == MODE_AF2) { // Alternate Function mode

        uint32_t MODER_CLR = ~((uint32_t) ((1<<(2*pin_number+1)) + (1<<(2*pin_number))));
        uint32_t MODER_ALT = 1<<(2*pin_number+1);
        reg_pointer = (uint32_t *)moder_register;
        *reg_pointer = *reg_pointer & MODER_CLR; // clear the bit
        *reg_pointer = *reg_pointer | MODER_ALT;

    } else {
        fprintf(stderr, "Invalid Mode");
    }

    /*Push-pull v open drain configuration */
    if(open_drain > 0) {
        uint16_t TYPER_OPEN_DR = ((uint16_t) 1<<pin_number);
        reg_pointer = (uint32_t *)otyper_register;
        *reg_pointer = *reg_pointer | TYPER_OPEN_DR; // don't need clear, just setting single bit
    } else {
        uint32_t TYPER_PUP = ~((uint32_t) 1<<(2*pin_number));
        uint32_t OTYPER_PP = 0x00;
        reg_pointer = (uint32_t *)otyper_register;
        *reg_pointer = *reg_pointer & ~TYPER_PUP; // Clear the bit
        *reg_pointer = *reg_pointer | OTYPER_PP; // Set the bit, although is just 0
    }

    /*Pin high speed */
    uint32_t OSPEEDR_HI = (uint32_t) ((1<<(2*pin_number+1)) + (1<<(2*pin_number)));
    reg_pointer = (uint32_t *)ospeedr_register;
    *reg_pointer = *reg_pointer | OSPEEDR_HI;

    reg_pointer = (uint32_t *)pupdr_register;
    uint32_t PUPD_FLOATING = 0; // Just 0
    uint32_t PUPD_CLR = ~((uint32_t) 0b11 << (2*pin_number));
    uint32_t PUPD_PU = (uint32_t) 1<<(2*pin_number);
    uint32_t PUPD_PD = (uint32_t) 1<<(2*pin_number+1);   
    /* pull up, pull down, neither config */
    switch(pupd) { 
        /* Configured neither, ie floating */
        case PUPD_FLOAT:           
            *reg_pointer = *reg_pointer & PUPD_CLR;
            *reg_pointer = *reg_pointer | PUPD_FLOATING;      
            break;
        /* Configured pull up */
        case PUPD_UP:
            *reg_pointer = *reg_pointer & PUPD_CLR;
            *reg_pointer = *reg_pointer | PUPD_PU;
            break;
        /* Configured pull-down */
        case PUPD_DOWN:
            *reg_pointer = *reg_pointer & PUPD_CLR;
            *reg_pointer = *reg_pointer | PUPD_PD;
            break;
        default:
            fprintf(stderr, "Pull Up Pull Down Setting Not Handled / Invalid");
            break;
    }

    /* Output Pin Init State Config */
    if(mode == MODE_OUT) {
        reg_pointer = (uint32_t *)odr_register;
        uint32_t ODR_CLR = ~((uint32_t) 1 << pin_number);
        uint32_t ODR_SET = ((uint32_t) 1 << pin_number);
        *reg_pointer = *reg_pointer & ODR_CLR;
        if(init_output > 0) *reg_pointer = *reg_pointer | ODR_SET;
    }

    /* Alternate Function Config */
    if ((mode == MODE_AF2) && (alt_func <= 15)) { // ALT FN MODE
        /* Set Port & Pin to the AF value passed to init */
        uint32_t AFR_CLR;
        uint32_t AFR_VAL;
        if (pin_number <= 7) { // AFRL register
            reg_pointer = (uint32_t *) afrl_register;
            AFR_CLR = ~((uint32_t) 0b1111<<(4*pin_number));
            AFR_VAL = alt_func<<(4*pin_number);
        }
        else {
            reg_pointer = (uint32_t *) afrh_register;
            AFR_CLR = ~((uint32_t) 0b1111<<(4*(pin_number-8)));
            AFR_VAL = alt_func<<(4*(pin_number-8));
        }

        *reg_pointer = *reg_pointer & AFR_CLR; // clear bit for alternate function
        *reg_pointer = *reg_pointer | AFR_VAL;

    } else if ((mode == MODE_AF2) && (alt_func > 15)) {
        fprintf(stderr, "Invalid alt function entry");
    }
     

}

/* *******************************************************************************
                    SET and CLEAR functions for GPIO outputs
   ******************************************************************************* */
void SETorCLEARGPIOoutput(int port_number, int pin_number, int set)
{
    uint32_t ODR_LO = ~((uint32_t) 1<<pin_number);
    uint32_t ODR_HI = (uint32_t) 1<<pin_number;

    uint32_t port_base_address = mapPortNumbertoBaseAddress(port_number); 
    uint32_t * reg_pointer = (uint32_t *)(long)(port_base_address + ODR_REGISTER_OFFSET);

    if (set) { // set pin HI
        *reg_pointer = *reg_pointer | ODR_HI;
    }
    else {
        *reg_pointer = *reg_pointer & ODR_LO;
    }    
}

void SETorCLEARGPIOpins(int port_numbers[], int pin_numbers[], int set_flags[], int num_pins)
{
    for(int i=0; i<num_pins; i++) {
        SETorCLEARGPIOoutput(port_numbers[i], pin_numbers[i], set_flags[i]); 
    }
}

/* *******************************************************************************
                    READ bit functions for GPIOC inputs
   ******************************************************************************* */
int readGPIOinput(int port_number, int pin_number)
{
    uint32_t value;
    uint32_t *reg_pointer;

    // access GPIOC_6 from the IDR register
    uint32_t IDR_STATUS = 1<<pin_number;
    uint32_t port_base_address = mapPortNumbertoBaseAddress(port_number); 
    reg_pointer = (uint32_t *)(long)(port_base_address + IDR_REGISTER_OFFSET);
    
    // Quaery its value using an AND operation
    value = *reg_pointer & IDR_STATUS; 
    return (value>0);  
}

int readGPIOoutput(int port_number, int pin_number)
{
    uint32_t value;
    uint32_t *reg_pointer;

    // access GPIOC_6 from the IDR register
    uint32_t ODR_STATUS = 1<<pin_number;
    uint32_t port_base_address = mapPortNumbertoBaseAddress(port_number); 
    reg_pointer = (uint32_t *)(long)(port_base_address + ODR_REGISTER_OFFSET);
    
    // Quaery its value using an AND operation
    value = *reg_pointer & ODR_STATUS; 
    return (value>0);  
}

void initGPIOasAnalog(int port_number, int pin_number) {
    uint32_t* reg_pointer;
    uint32_t port_base_address = mapPortNumbertoBaseAddress(port_number);
    // 1. Enable the AHB1 clock
    enableAHB1GPIOclock(port_number);
    // 2. Setup GPIO pin as analog by writing setting bits 14 and 15 in the MODER register
    reg_pointer = (uint32_t*) (uint32_t *)(long)(port_base_address + MODER_REGISTER_OFFSET);
    *reg_pointer = *reg_pointer | ((uint32_t) (0b11 << (pin_number*2)));
    // 3. Setup GPIO pin to be a push-pull output by writing to the OTYPER register
    reg_pointer = (uint32_t *)(long)(port_base_address + OTYPER_REGISTER_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(0x01 << pin_number))); // First clear bit
    *reg_pointer = *reg_pointer | 0x00; // Set bit
    // 4. Setup GPIO pin to be floating by writing to the PUPDR register
    reg_pointer = (uint32_t*)(long)(port_base_address + PUPDR_REGISTER_OFFSET);
    *reg_pointer = *reg_pointer & (~((uint32_t)(0b11 << (pin_number*2))));
}

/* *******************************************************************************
                    GPIO UTILITY FUNCTIONS
   ******************************************************************************* */
uint32_t mapPortNumbertoBaseAddress(int port_number)
{
    uint32_t port_base_address;
    switch (port_number) {
        case 0 : {port_base_address = PORTA_BASE_ADDRESS; break;}
        case 1 : {port_base_address = PORTB_BASE_ADDRESS; break;}
        case 2 : {port_base_address = PORTC_BASE_ADDRESS; break;}
        case 3 : {port_base_address = PORTD_BASE_ADDRESS; break;}
        case 4 : {port_base_address = PORTE_BASE_ADDRESS; break;}
        case 5 : {port_base_address = PORTF_BASE_ADDRESS; break;}
        case 6 : {port_base_address = PORTG_BASE_ADDRESS; break;}
        case 7 : {port_base_address = PORTH_BASE_ADDRESS; break;}
        default : fprintf(stderr, "Received Unknown Port Number at Base Address Map");
    }

    return port_base_address;
}

void enableAHB1GPIOclock(int port_number)
{
    switch (port_number) {
        case 0 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); break;}
        case 1 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); break;}
        case 2 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); break;}
        case 3 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); break;}
        case 4 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); break;}
        case 5 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); break;}
        case 6 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); break;}
        case 7 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); break;}
        default : fprintf(stderr, "Received Unknown Port Number at AHB1 clock enable");
    }

}
