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
#include "state_machine_SPI.h"
#include "stdio.h"
#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_interrupt.h"
#include <cstdint>

/* Macro Definitions */
// SPI register addresses
#define SPI1_BASE_ADDRESS ((uint32_t)0x40013000)
#define SPI2_BASE_ADDRESS ((uint32_t)0x40003800)
#define SPI3_BASE_ADDRESS ((uint32_t)0x40003C00)
#define SPI4_BASE_ADDRESS ((uint32_t)0x40013400)

// SPI offsets
#define SPI_CONTROL_REGISTER1_OFFSET ((uint32_t)0x00)
#define SPI_CONTROL_REGISTER2_OFFSET ((uint32_t)0x04)
#define SPI_STATUS_REGISTER_OFFSET ((uint32_t)0x08)
#define SPI_DATA_REGISTER_OFFSET ((uint32_t)0x0c)
#define SPI_CRC_POLYNOMIAL_REGISTER_OFFSET ((uint32_t)0x10)
#define SPI_RX_CRC_REGISTER_OFFSET ((uint32_t)0x14)
#define SPI_TX_CRC_REGISTER_OFFSET ((uint32_t)0x18)

// SPI reset masks
#define SPI_CONTROL_REGISTER1_RESET_MASK ~((uint32_t)0xffff)
#define SPI_CONTROL_REGISTER2_RESET_MASK ~((uint32_t)0b11110111)
#define SPI_STATUS_REGISTER_RESET_MASK ~((uint32_t)0b11111101) // reset value for bit 1 is 1
#define SPI_DATA_REGISTER_RESET_MASK ~((uint32_t)0xffff)

// CS pins for PARENT_SENSOR spi 
const uint8_t CS_PINS[6] = {
    PORT_G, PIN_2, // CS[0] is pin G2
    PORT_G, PIN_3, // CS[1] is pin G3
    PORT_F, PIN_2 // CS[2] is pin F2
}; // support 3 devices for now

void configureSPIPeripheral(Spi_Hierarchy_t spi_type, uint8_t spi_id){
    /*  configure SPI as parent 
        using a base clock speed of 45MHz
    */
    
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* control_register1_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER1_OFFSET);
    uint32_t* control_register2_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER2_OFFSET);


    switch(spi_id){
        // enable SPI clock bus (APB2 for SPI1/4)
        case 1: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); break;
        case 4: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE); break;
        default: fprintf(stderr, "Tried to init with invalid/unsupported SPI ID"); break;
    }

    // disable SPI
    *control_register1_addr = *control_register1_addr & ~(uint16_t)(0b1 << 6);

    // 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
    switch(spi_id){
        case 1:
            // PICO pin A6 -> moder 2 for alt, open drain, neither PUPD, alt func = 5
            initGPIOasMode(PORT_A, PIN_6, MODE_AF, OD_OPEN_DRAIN, PUPD_FLOAT, 0, 5);
            // POCI pin A7 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
            initGPIOasMode(PORT_A, PIN_7, MODE_AF, OD_PUSH_PULL, PUPD_FLOAT, 0, 5);
            // SCLK pin B3 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
            initGPIOasMode(PORT_B, PIN_3, MODE_AF, OD_PUSH_PULL, PUPD_FLOAT, 0, 5);
            // CS pin(s) A15 -> moder 1 for out, push pull, PU, ODR high
            initGPIOasMode(PORT_A, PIN_15, MODE_OUT, OD_PUSH_PULL, PUPD_UP, 1, 0);
        break;
        case 4:
            //PICO/MISO pin PE5 ->  moder 2 for alt, open drain, neither PUPD, alt func = 5
            initGPIOasMode(PORT_E, PIN_5, MODE_AF, OD_OPEN_DRAIN, PUPD_FLOAT, 0, 5);
            //POCI/MOSI pin PE6 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
            initGPIOasMode(PORT_E, PIN_6, MODE_AF, OD_PUSH_PULL, PUPD_FLOAT, 0, 5);
            // SCLK pin PE2 -> moder 2 for alt, push-pull, neither PUPD, alt func = 5
            initGPIOasMode(PORT_E, PIN_2, MODE_AF, OD_PUSH_PULL, PUPD_FLOAT, 0, 5);
            // CS pin PE4 -> moder 1 for out, push pull, PU, ODR high
            initGPIOasMode(PORT_E, PIN_4, MODE_OUT, OD_PUPD, PUPD_UP, 1, 0);
            // CS pins for sensors, as defined in CS_PINS in hardware_stm_spi.h
            for(int i = 0; i < 3; i++){
                initGPIOasMode(CS_PINS[i/2], CS_PINS[i/2+1], MODE_OUT, OD_PUPD, PUPD_UP, 1, 0);
            }
        break;
        default:
            fprintf(stderr, "Tried to init with invalid SPI ID");
        break;

    }
    
    //2. Write to the SPI_CR1 register:
    *control_register1_addr = *control_register2_addr & SPI_CONTROL_REGISTER1_RESET_MASK;
    // spi role specific settings/declarations
    uint16_t baudrate_bits;
    uint16_t cpol_cpha_bits;
    uint16_t lsbfirst_bit;
    if(spi_id == 4) // nucleo - sensor comm
    {
        // set baud rate
        baudrate_bits = (uint16_t)(0b011 << 3); // fpclk/16 = 5.6 MHz (fastest speed below 8 MHz with available prescaler)
        baudrate_bits = (uint16_t)(0b100 << 3); // trying at slower baud rates to debug
        // depth sensor looking for CPOL = 1 and CPHA = 1
        cpol_cpha_bits = (uint16_t)(0b11);
        // bit order, depth sensor transmists most significant first (same as other case actually)
        lsbfirst_bit = (uint16_t)(0b0 << 7); // most significant bit first
    }
    else // nucleo - nucleo comm
    {
        //a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 3).
        baudrate_bits = (uint16_t)(0b000 << 3); //fpclk/2 = 45MHz
        //b) Configure the CPOL and CPHA bits combination to define one of the four
        //relationships between the data transfer and the serial clock. (Note: 2 - except the
        //case when CRC is enabled at TI mode).
        cpol_cpha_bits = (uint16_t)(0b00); 
        // d) Configure the LSBFIRST bit to define the frame format (Note: 2).
        lsbfirst_bit = (uint16_t)(0b0 << 7); // most significant bit first for sx1276               
    }
    
    // c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
    // BIDIOE (RXONLY and BIDIMODE can't be set at the same time).
    //  we want full duplex, so don't write anything
    // e) Configure the CRCEN and CRCEN bits if CRC is needed (while SCK clock signal
    // is at idle state).
    //  not implementing CRC for now
    // f) Configure SSM and SSI (Note: 2).
    uint16_t software_cs_bits;
    if(spi_type==SPI_PARENT || spi_type==SENSOR_PARENT) software_cs_bits = (uint16_t)(0b11 << 8); // manage cs with software
    if(spi_type==SPI_CHILD) software_cs_bits = (uint16_t)(0b10 << 8); 
    // g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
    // NSS if parent is configured to prevent MODF error).
    uint16_t parent_bit;
    if(spi_type==SPI_PARENT || spi_type==SENSOR_PARENT) {parent_bit = (uint16_t)(0b100);}
    if(spi_type==SPI_CHILD) {parent_bit = (uint16_t)(0b000);}
    
    // h) Set the DFF bit to configure the data frame format (8 or 16 bits).
    uint16_t dff_bit = (uint16_t)(0b1 << 11); // 16 bits

    // finally, write all that to SPI_CR1
    *control_register1_addr = baudrate_bits | cpol_cpha_bits | lsbfirst_bit | 
        software_cs_bits | parent_bit | dff_bit;

    // debugging
    //printf("SPI_CR1 reads %u\n", *control_register1_addr);
    
    //3. Write to SPI_CR2 register:

    // a) Configure SSOE (Note: 1 & 2).
    *control_register2_addr = *control_register2_addr & SPI_CONTROL_REGISTER2_RESET_MASK;
    uint16_t SSOE_MASK = (uint16_t)(0b0 << 2); // set SSOE to 0

    // enable interrupts on TXE=1 (bit 7) and RXNE=1 (bit 6) in SPI_CR2
    // but don't enable TXE interrupts in init, enable them on transmission to avoid runaway interrupts
    uint16_t INTERRUPT_MASK = (uint16_t)(0b1 << 6);

    // set SPI_CR2
    *control_register2_addr = *control_register2_addr | SSOE_MASK | INTERRUPT_MASK;

    //printf("SPI_CR2 reads %u\n", *control_register2_addr);

    // Enable the interrupts in NVIC
    switch (spi_id){
        case 1: 
            enableSPI1Interrupt();
            break;
        case 4: 
            enableSPI4Interrupt();
            break;
    }

    // finally, enable the SPI in SPI control register 1
    *control_register1_addr = *control_register1_addr | (uint16_t)(0b1 << 6);
    //printf("SPI_CR1 reads %u\n", *control_register1_addr);
}

void writeTX(uint8_t spi_id, uint16_t value)
{
    /* Write to the TX buffer of an SPI */
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* data_register_address = (uint32_t*)(long)
                (base_address + SPI_DATA_REGISTER_OFFSET);
    *data_register_address = value;
    //printf("write %u \n", value);
}

uint16_t readRX(uint8_t spi_id)
{
    /* Read from the RX buffer of an SPI */
    uint32_t base_address = getSPIBaseAddr(spi_id);

    uint32_t* data_register_address = (uint32_t*)(long)
                (base_address + SPI_DATA_REGISTER_OFFSET);
    return *data_register_address;
}

void enableSpiTXEInterrupts(uint8_t spi_id){
    // enable the SPI TXE interrupt for SPIi, where i = spi_id
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* control_register2_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER2_OFFSET);
    uint16_t ENABLE_TXE_MASK = 0b1 << 7;
    *control_register2_addr = *control_register2_addr | ENABLE_TXE_MASK;
}

void disableSpiTXEInterrupts(uint8_t spi_id){
    // disable the SPI TXE interrupt for SPIi, where i = spi_id
    uint32_t base_address = getSPIBaseAddr(spi_id);
    uint32_t* control_register2_addr = (uint32_t *)(long)(base_address + SPI_CONTROL_REGISTER2_OFFSET);
    uint16_t ENABLE_TXE_MASK = 0b1 << 7;
    *control_register2_addr = *control_register2_addr & ~ENABLE_TXE_MASK;
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
        default:
            fprintf(stderr, "Tried to get register address for invalid SPI ID");
        break;
    }
    return port_base_address;
}


