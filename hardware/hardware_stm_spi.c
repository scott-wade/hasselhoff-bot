/**
  ******************************************************************************
  * @file    hardware_stm_spi.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November 2024
  * @brief   STM32F446ze SPI
  ******************************************************************************
  */

#include "hardware_stm_spi.h"
#include "hardware_stm_gpio.h"
#include "stdio.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>


/* Macro Definitions */
// SPI register addresses
#define SPI1_BASE_ADDRESS ((uint32_t) 0x40013000)
#define SPI2_BASE_ADDRESS ((uint32_t) 0x40003800)
#define SPI3_BASE_ADDRESS ((uint32_t) 0x40003C00)
#define SPI4_BASE_ADDRESS ((uint32_t) 0x40013400)


// SPI offsets
#define SPI_CONTROL_REGISTER1_OFFSET ((uint32_t) 0x00)
#define SPI_CONTROL_REGISTER2_OFFSET ((uint32_t) 0x04)
#define SPI_STATUS_REGISTER_OFFSET ((uint32_t) 0x08)
#define SPI_DATA_REGISTER_OFFSET ((uint32_t) 0x0c)
#define SPI_CRC_POLYNOMIAL_REGISTER_OFFSET ((uint32_t) 0x10)
#define SPI_RX_CRC_REGISTER_OFFSET ((uint32_t) 0x14)
#define SPI_TX_CRC_REGISTER_OFFSET ((uint32_t) 0x18)

// SPI reset masks
#define SPI_CONTROL_REGISTER1_RESET_MASK ~((uint32_t)0xffff)
#define SPI_CONTROL_REGISTER2_RESET_MASK ~((uint32_t)0b11110111)
#define SPI_STATUS_REGISTER_RESET_MASK ~((uint32_t)0b11111101) // reset value for bit 1 is 1
#define SPI_DATA_REGISTER_RESET_MASK ~((uint32_t)0xffff)



void configureSPI(void){
    /* configure SPI 1 */





}
