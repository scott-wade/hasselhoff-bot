/**
 ******************************************************************************
 * @file    hardware_stm_spi.c
 * @author  scottwad@andrew.cmu.edu
 * @version 1.0
 * @date    November 2024
 * @brief   STM32F446ze SPI
 ******************************************************************************
 */

// Page 887

#include "hardware_stm_spi.h"
#include "hardware_stm_gpio.h"
#include "stdio.h"
#include "stm32f4xx_rcc_mort.h"
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

typedef struct
{
    uint8_t id;
    uint16_t timeout;
    int CS_PORT_NUM;
    int CS_PIN_NUM;
} SPI_t;

void configureSPI(uint8_t spi_id)
{
    /* configure SPI as parent */

    uint32_t *base_address = getSPIBaseAddr(spi_id);
    uint32_t *control_register1_addr = base_address + SPI_CONTROL_REGISTER1_OFFSET;
    uint32_t *control_register2_addr = base_address + SPI_CONTROL_REGISTER2_OFFSET;

    // enable SPI clock bus

    // disable SPI
    *control_register1_addr = *control_register1_addr & ~(uint16_t)(0b1 << 6);

    // 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.

    // 2. Write to the SPI_CR1 register:
    *control_register1_addr = *control_register2_addr & SPI_CONTROL_REGISTER1_RESET_MASK;
    // a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 3).
    uint16_t baudrate_bits = (uint16_t)(0b000 << 3); // fpclk/2
    // b) Configure the CPOL and CPHA bits combination to define one of the four
    // relationships between the data transfer and the serial clock. (Note: 2 - except the
    // case when CRC is enabled at TI mode).
    uint16_t cpol_cpha_bits = (uint16_t)(0b00);
    // c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
    // BIDIOE (RXONLY and BIDIMODE can't be set at the same time).
    //  we want full duplex, so don't write anything
    // d) Configure the LSBFIRST bit to define the frame format (Note: 2).
    uint16_t lsbfirst_bit = (uint16_t)(0b0 << 7); // most significant bit first for sx1276
    // e) Configure the CRCEN and CRCEN bits if CRC is needed (while SCK clock signal
    // is at idle state).
    //  not implementing CRC for now
    // f) Configure SSM and SSI (Note: 2).
    uint16_t software_cs_bits = (uint16_t)(0b11 << 8); // manage cs with software
    // g) Configure the MSTR bit (in multimaster NSS configuration, avoid conflict state on
    // NSS if parent is configured to prevent MODF error).
    uint16_t parent_bit = (uint16_t)(0b100);
    // h) Set the DFF bit to configure the data frame format (8 or 16 bits).
    uint16_t dff_bit = (uint16_t)(0b1 << 11); // 16 bits

    // finally, write all that to SPI_CR1
    *control_register1_addr = baudrate_bits | cpol_cpha_bits | lsbfirst_bit |
                              software_cs_bits | parent_bit | dff_bit;

    // 3. Write to SPI_CR2 register:

    // a) Configure SSOE (Note: 1 & 2).
    *control_register2_addr = *control_register2_addr & SPI_CONTROL_REGISTER2_RESET_MASK;
    *control_register2_addr = *control_register2_addr | (uint16_t)(0b0 << 2); // set SSOE to 0

    // b) Set the FRF bit if the TI protocol is required.
    //  not implementing TI protocol
    // 4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed.
    //  not implementing CRC

    // do we need to reset the status register?

    // finally, enable the SPI in SPI control register 1
    *control_register1_addr = *control_register1_addr | (uint16_t)(0b1 << 6);
}

void writeTX(SPI_t spi, uint16_t value)
{
    // Toggle CS
    SETorCLEARGPIOoutput(spi.CS_PORT_NUM, spi.CS_PIN_NUM, 0);
    /* Write to the TX buffer of an SPI */
    uint32_t *base_address = getSPIBaseAddr(spi.spi_id);
    uint32_t *data_register_address = (uint32_t *)(base_address + SPI_DATA_REGISTER_OFFSET);
    *data_register_address = value;
    // wait for transmission to finish
    while (!transmissionFinished(spi))
    {
        delay(1);
    }
    // Toggle CS
    SETorCLEARGPIOoutput(spi.CS_PORT_NUM, spi.CS_PIN_NUM, 1);
}

void writeTX(SPI_t spi, uint32_t *value)
{
    // Toggle CS
    SETorCLEARGPIOoutput(spi.CS_PORT_NUM, spi.CS_PIN_NUM, 0);
    /* Write to the TX buffer of an SPI */
    uint32_t *base_address = getSPIBaseAddr(spi.spi_id);
    uint32_t *data_register_address = (uint32_t *)(base_address + SPI_DATA_REGISTER_OFFSET);
    memset(data_register_address, value, sizeof(uint32_t));
    // *data_register_address = &value;
    // wait for transmission to finish
    while (!transmissionFinished(spi))
    {
        delay(1);
    }
    // Toggle CS
    SETorCLEARGPIOoutput(spi.CS_PORT_NUM, spi.CS_PIN_NUM, 1);
}

bool transmissionFinished(SPI_t spi)
{
    /* Check if the transmission is finished */
    uint32_t *base_address = getSPIBaseAddr(spi.spi_id);
    uint32_t *status_register_address = (uint32_t *)(base_address + SPI_STATUS_REGISTER_OFFSET);
    uint32_t status_register = *status_register_address;
    return (status_register & 0b10) == 0;
}

bool dataAvailable(SPI_t spi)
{
    /* Check if data is available to be read */
    uint32_t *base_address = getSPIBaseAddr(spi.spi_id);
    uint32_t *status_register_address = (uint32_t *)(base_address + SPI_STATUS_REGISTER_OFFSET);
    uint32_t status_register = *status_register_address;
    return (status_register & 0b1) == 1;
}

uint16_t readRX(SPI_t spi)
{
    // Check if data is ready to be read
    uint16_t timeout;
    while (!dataAvailable(spi.spi_id))
    {
        delay(1);
        timeout++;
        if (timeout > spi.timeout)
        {
            return 0;
        }
    }
    /* Read from the RX buffer of an SPI */
    uint32_t *base_address = getSPIBaseAddr(spi_id);
    uint32_t *data_register_address = (uint32_t *)(base_address + SPI_DATA_REGISTER_OFFSET);
    return *data_register_address;
}

uint32_t *getSPIBaseAddr(uint8_t spi_id)
{
    // the SPI base address based on the SPI id
    uint32_t *port_base_address;
    switch (spi_id)
    {
    case 1:
        port_base_address = (uint32_t *)SPI1_BASE_ADDRESS;
    case 2:
        port_base_address = (uint32_t *)SPI2_BASE_ADDRESS;
    case 3:
        port_base_address = (uint32_t *)SPI3_BASE_ADDRESS;
    case 4:
        port_base_address = (uint32_t *)SPI4_BASE_ADDRESS;
    }
    return port_base_address;
}