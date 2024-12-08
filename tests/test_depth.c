/**
 ******************************************************************************
 * @file    hardware_lps_spi.c
 * @author  nathanz@andrew.cmu.edu
 * @version 1.0
 * @date    December 2024
 * @brief   LPS27HHW SPI
 ******************************************************************************
 */

 #include "test_depth.h"
 #include "hardware/hardware_lps_spi.h"
 #include "hardware/hardware_stm_gpio.h"
 #include "hardware/hardware_stm_interrupt.h"

/* MACRO definitions----------------------------------------------------------*/
 
/* Declarations ----------------------------------------------------------*/

/* Functions. ----------------------------------------------------------*/
 // setup the button gpio as an external interrupt input
 void initButtonIntInput(void)
 {
    // just hooking the button up in a pulled up setup (ie looking for a falling edge)
    enableEXTI6OnPortC();
 } 