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

void configureSPIParent(uint8_t spi_id){
    /*  configure SPI as parent 
        using a base clock speed of 45MHz
    */
    
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* control_register1_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER1_OFFSET);
    uint32_t* control_register2_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER2_OFFSET);

    // enable SPI clock bus (APB2)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    // disable SPI
    *control_register1_addr = *control_register1_addr & ~(uint16_t)(0b1 << 6);

    // 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
    // PICO pin A6 -> moder 2 for alt, open drain, neither PUPD, alt func = 5
    initGPIOasMode(0, 6, 2, 1, 0, 0, 5);
    // POCI pin A7 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
    initGPIOasMode(0, 7, 2, 0, 0, 0, 5);
    // SCLK pin A5 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
    initGPIOasMode(0, 5, 2, 0, 0, 0, 5);
    // CS pin(s) A4 -> moder 1 for out, push pull, PU, ODR high
    initGPIOasMode(0, 4, 1, 0, 1, 1, 0);


    //2. Write to the SPI_CR1 register:
    *control_register1_addr = *control_register2_addr & SPI_CONTROL_REGISTER1_RESET_MASK;
    //a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 3).
    uint16_t baudrate_bits = (uint16_t)(0b000 << 3); //fpclk/2 = 45MHz
    //b) Configure the CPOL and CPHA bits combination to define one of the four
    //relationships between the data transfer and the serial clock. (Note: 2 - except the
    //case when CRC is enabled at TI mode).
    uint16_t cpol_cpha_bits = (uint16_t)(0b00);
    //c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
    //BIDIOE (RXONLY and BIDIMODE can't be set at the same time).
        // we want full duplex, so don't write anything
    //d) Configure the LSBFIRST bit to define the frame format (Note: 2).
    uint16_t lsbfirst_bit = (uint16_t)(0b0 << 7);//most significant bit first for sx1276
    //e) Configure the CRCEN and CRCEN bits if CRC is needed (while SCK clock signal
    //is at idle state).
        // not implementing CRC for now
    //f) Configure SSM and SSI (Note: 2).
    uint16_t software_cs_bits = (uint16_t)(0b11 << 8); //manage cs with software
    //g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
    //NSS if parent is configured to prevent MODF error).
    uint16_t parent_bit = (uint16_t)(0b100);
    //h) Set the DFF bit to configure the data frame format (8 or 16 bits).
    uint16_t dff_bit = (uint16_t)(0b1 << 11); // 16 bits

    // finally, write all that to SPI_CR1
    *control_register1_addr = baudrate_bits | cpol_cpha_bits | lsbfirst_bit | 
        software_cs_bits | parent_bit | dff_bit;

    
    //3. Write to SPI_CR2 register:

    //a) Configure SSOE (Note: 1 & 2).
    *control_register2_addr = *control_register2_addr & SPI_CONTROL_REGISTER2_RESET_MASK;
    *control_register2_addr = *control_register2_addr | (uint16_t)(0b0 << 2); // set SSOE to 0

    printf("SPI_CR2 reads %u\n", *control_register2_addr);
    //b) Set the FRF bit if the TI protocol is required.
        // not implementing TI protocol
    //4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed.
        // not implementing CRC


    // do we need to reset the status register?

    // finally, enable the SPI in SPI control register 1
    *control_register1_addr = *control_register1_addr | (uint16_t)(0b1 << 6);
    printf("SPI_CR1 reads %u\n", *control_register1_addr);

}



void writeTX(uint8_t spi_id, uint16_t value){
    /* Write to the TX buffer of an SPI */
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* data_register_address = (uint32_t*)(long)
                (base_address + SPI_DATA_REGISTER_OFFSET);
    *data_register_address = value;
}

uint16_t readRX(uint8_t spi_id){
    /* Read from the RX buffer of an SPI */
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* data_register_address = (uint32_t*)(long)
                (base_address + SPI_DATA_REGISTER_OFFSET);
    return *data_register_address;
}

uint16_t readSpiStatusRegister(uint8_t spi_id){
    /*Read status register of an SPI*/
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* status_register_address = (uint32_t*)(long)(base_address + SPI_STATUS_REGISTER_OFFSET);
    return *status_register_address;
}

void resetSpiRXNE(uint8_t spi_id){
    /* reset the RXNE status register flag */
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* status_register_address = (uint32_t*)(long)(base_address + SPI_STATUS_REGISTER_OFFSET);
    *status_register_address = *status_register_address & ~((uint16_t)0b0);
}

uint32_t getSPIBaseAddr(uint8_t spi_id){
    // the SPI base address based on the SPI id
    uint32_t port_base_address;
    switch(spi_id){
        case 1: 
            port_base_address = (uint32_t)SPI1_BASE_ADDRESS;
            break;
        case 2: 
            port_base_address = (uint32_t)SPI2_BASE_ADDRESS;
            break;
        case 3: 
            port_base_address = (uint32_t)SPI3_BASE_ADDRESS;
            break;
        case 4: 
            port_base_address = (uint32_t)SPI4_BASE_ADDRESS;
            break;
    }
    return port_base_address;
}