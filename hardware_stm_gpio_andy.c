/**
  ******************************************************************************
  * @file    hardware_stm_gpio.c 
  * @author  mortamar@andrew.cmu.edu
  * @version 1.0
  * @date    Septembr-2021
  * @brief   Controls STM32F446ze GPIO
  ******************************************************************************
  */

#include "hardware_stm_gpio_andy.h"
#include "stm32f4xx_rcc_mort.h"



/* MACRO definitions----------------------------------------------------------*/

//Port C Addresses: 
#define PORTC_BASE_ADDRESS  ((uint32_t)0x40020800)
#define PORTC_MODER_REGISTER (PORTC_BASE_ADDRESS + 0x00)
#define PORTC_OTYPER_REGISTER (PORTC_BASE_ADDRESS + 0x04)
#define PORTC_OSPEEDR_REGISTER (PORTC_BASE_ADDRESS + 0x08)
#define PORTC_PUPDR_REGISTER (PORTC_BASE_ADDRESS + 0x0C)
#define PORTC_IDR_REGISTER (PORTC_BASE_ADDRESS + 0x10)
#define PORTC_ODR_REGISTER (PORTC_BASE_ADDRESS + 0x14)
#define PORTC_BSRRL_REGISTER (PORTC_BASE_ADDRESS + 0x18)
#define PORTC_BSRRH_REGISTER (PORTC_BASE_ADDRESS + 0x1A)
#define PORTC_LCKR_REGISTER (PORTC_BASE_ADDRESS + 0x1C)
#define PORTC_AFR1_REGISTER (PORTC_BASE_ADDRESS + 0x20)
#define PORTC_AFR2_REGISTER (PORTC_BASE_ADDRESS + 0x24)

//Port D Addresses: 
#define PORTD_BASE_ADDRESS ((uint32_t)0x40020C00)
#define PORTD_MODER_REGISTER (PORTD_BASE_ADDRESS + 0x00)
#define PORTD_OTYPER_REGISTER (PORTD_BASE_ADDRESS + 0x04)
#define PORTD_OSPEEDR_REGISTER (PORTD_BASE_ADDRESS + 0x08)
#define PORTD_PUPDR_REGISTER (PORTD_BASE_ADDRESS + 0x0C)
#define PORTD_IDR_REGISTER (PORTD_BASE_ADDRESS + 0x10)
#define PORTD_ODR_REGISTER (PORTD_BASE_ADDRESS + 0x14)
#define PORTD_BSRRL_REGISTER (PORTD_BASE_ADDRESS + 0x18)
#define PORTD_BSRRH_REGISTER (PORTD_BASE_ADDRESS + 0x1A)
#define PORTD_LCKR_REGISTER (PORTD_BASE_ADDRESS + 0x1C)
#define PORTD_AFR1_REGISTER (PORTD_BASE_ADDRESS + 0x20)
#define PORTD_AFR2_REGISTER (PORTD_BASE_ADDRESS + 0x24)

//Port E Addresses: 
#define PORTE_BASE_ADDRESS ((uint32_t) 0x40021000)
#define PORTE_MODER_REGISTER (PORTE_BASE_ADDRESS + 0x00)
#define PORTE_OTYPER_REGISTER (PORTE_BASE_ADDRESS + 0x04)
#define PORTE_OSPEEDR_REGISTER (PORTE_BASE_ADDRESS + 0x08)
#define PORTE_PUPDR_REGISTER (PORTE_BASE_ADDRESS + 0x0C)
#define PORTE_IDR_REGISTER (PORTE_BASE_ADDRESS + 0x10)
#define PORTE_ODR_REGISTER (PORTE_BASE_ADDRESS + 0x14)
#define PORTE_BSRRL_REGISTER (PORTE_BASE_ADDRESS + 0x18)
#define PORTE_BSRRH_REGISTER (PORTE_BASE_ADDRESS + 0x1A)
#define PORTE_LCKR_REGISTER (PORTE_BASE_ADDRESS + 0x1C)
#define PORTE_AFR1_REGISTER (PORTE_BASE_ADDRESS + 0x20)
#define PORTE_AFR2_REGISTER (PORTE_BASE_ADDRESS + 0x24)

//flags MODER Register:
#define GPIO_0_MODER 0x03
#define GPIO_0_MODER_OUT 0x01
#define GPIO_0_MODER_AF 0x02
#define GPIO_1_MODER (uint32_t)(0x03<<2)
#define GPIO_1_MODER_OUT (uint32_t) (0x01<<2)
#define GPIO_1_MODER_AF (uint32_t)(0x02<<2)
#define GPIO_2_MODER (uint32_t) (0x03<<4)
#define GPIO_2_MODER_OUT (uint32_t) (0x01<<4)
#define GPIO_2_MODER_AF (uint32_t) (0x02<<4)
#define GPIO_3_MODER (uint32_t) (0x03<<6)
#define GPIO_3_MODER_OUT (uint32_t) (0x01<<6)
#define GPIO_3_MODER_AF (uint32_t) (0x02<<6)
#define GPIO_4_MODER (uint32_t) (0x03<<8)
#define GPIO_4_MODER_OUT (uint32_t) (0x01<<8)
#define GPIO_4_MODER_AF (uint32_t) (0x02<<8)
#define GPIO_5_MODER (uint32_t) (0x03<<10)
#define GPIO_5_MODER_OUT (uint32_t) (0x01<<10)
#define GPIO_5_MODER_AF (uint32_t) (0x02<<10)
#define GPIO_6_MODER 0x3000
#define GPIO_6_MODER_AF 0x2000
#define GPIO_6_MODER_OUT 0x1000
#define GPIO_6_MODER_IN 0x0000
#define GPIO_7_MODER (uint32_t)(0x03<<14)
#define GPIO_7_MODER_OUT (uint32_t) (0x01<<14)
#define GPIO_7_MODER_AF (uint32_t) (0x02<<14)
#define GPIO_8_MODER (uint32_t)(0x03<<16)
#define GPIO_8_MODER_OUT (uint32_t) (0x01<<16)
#define GPIO_8_MODER_AF (uint32_t) (0x02<<16)
#define GPIO_9_MODER (uint32_t)(0x03<<18)
#define GPIO_9_MODER_OUT (uint32_t)(0x01<<18)
#define GPIO_9_MODER_AF (uint32_t)(0x02<<18)
#define GPIO_10_MODER (uint32_t)(0x03<<20)
#define GPIO_10_MODER_OUT (uint32_t)(0x01<<20)
#define GPIO_10_MODER_AF (uint32_t)(0x02<<20)
//flags OTYPER Register:
#define GPIO_0_OTYPER 0x01
#define GPIO_OTYPER_PP 0x00
#define GPIO_1_OTYPER (uint32_t)(0x01<<1)
#define GPIO_1_OTYPER_PP 0x00
#define GPIO_2_OTYPER (uint32_t) (0x01<<2)
#define GPIO_2_OTYPER_PP 0x00
#define GPIO_3_OTYPER (uint32_t)(0x01<<3)
#define GPIO_3_OTYPER_pp 0x00
#define GPIO_4_OTYPER (uint32_t)(0x01<<4)
#define GPIO_4_OTYPER_PP 0x00
#define GPIO_5_OTYPER (uint32_t)(0x01<<5)
#define GPIO_5_OTYPER_PP 0x00
#define GPIO_6_OTYPER 0x40
#define GPIO_6_OTYPER_PP 0x00
#define GPIO_7_OTYPER (uint32_t)(0x01<<7)
#define GPIO_7_OTYPER_PP 0x00
#define GPIO_8_OTYPER (uint32_t)(0x01<<8)
#define GPIO_8_OTYPER_PP 0x00
#define GPIO_9_OTYPER (uint32_t) (0x01<<9)
#define GPIO_9_OTYPER_PP 0x00
#define GPIO_10_OTYPER (uint32_t)(0x01<<10)
#define GPIO_10_OTYPER_PP 0x00

//flags OSPEEDR Register:
#define GPIO_0_OSPEEDR_HIGH_SPEED 3
#define GPIO_6_OSPEEDR 0x3000
#define GPIO_6_OSPEEDR_HIGH_SPEED 0x3000

//flags PUPDR Register:


//input data register:

//flags AFR1 Register:

//flags ODR Register:


/* function definitions----------------------------------------------------------*/

void initGpioC6AsInput( void )
{
      uint32_t  * reg_pointer; 
      /* GPIOC Peripheral clock enable */
   
    /* GPIOC Pin 6 as input*/
    
    /*PUSH-PULL Pin*/
    
    /*GPIOC pin 6 high speed */
    
    /*Configure pulled-down*/
    
}


void initGpioB0AsOutput( void )
{
      uint32_t  * reg_pointer;
      /* GPIOB Peripheral clock enable */
    
    /* GPIOB0 configured as output */
    
    /*GPIOB0 configured as push-pull */
    
    /*GPIOB0 configured floating */
    
    /* GPIOB0 driven high to start out with */
        
}



void toggleGPIOB0( void )
{
    uint32_t value;
    uint32_t  * reg_pointer;
    //get the current value of the pin 
   
    if (value > 0)
    {
        //if high, clear the bit
        
    }
    else
    {
        //if low, set the bit
       
    } 
}



void setGPIOB0( void )
{
           
}
void clearGPIOB0( void )
{
         
}

uint32_t checkGPIOC6(void)
{
 uint32_t valueC6;

 return valueC6;   
}