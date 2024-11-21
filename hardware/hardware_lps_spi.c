/**
 ******************************************************************************
 * @file    hardware_lps_spi.c
 * @author  nathanz@andrew.cmu.edu
 * @version 1.0
 * @date    November 2024
 * @brief   LPS27HHW SPI
 ******************************************************************************
 */

#include "hardware_lps_spi.h"
#include "stm32f4xx_rcc_mort.h"


/* MACRO definitions----------------------------------------------------------*/
#define IF_CTRL                 0x0E
#define CTRL_REG1               0x10
#define CTRL_REG2               0x11
#define CTRL_REG3               0x12
#define FIFO_CTRL               0x13
#define FIFO_WTM                0x14
#define STATUS                  0x27
#define PRESSURE_OUT_XL         0x28
#define PRESSURE_OUT_L          0x29
#define PRESSURE_OUT_H          0x2A
#define TEMP_OUT_L              0x2B
#define TEMP_OUT_H              0x2C

/* Flag / Settings ----------------------------------------------------------*/
#define READ                    0b1 << 7
#define WRITE                   0b0 << 0
// CTRL REG1 FLAGS
#define DATA_RATE               0b101 << 4  // 75 Hz max data rate (though this gets halved at min regardless)
#define EN_LPFP                 0b0 << 3    // even with filters off, can still get really low noise
#define LPFP_CFG                0b0 << 2
#define BDU                     0b0 << 1
#define SIM                     0b0
// CTRL REG2 FLAGS
#define LOW_NOISE_EN            0b1 << 1

/* Declarations ----------------------------------------------------------*/
static uint8_t highPressureReg;
static uint8_t lowPressureReg;
static uint8_t xlowPressureReg;

/* Functions. ----------------------------------------------------------*/
// function initializes the LPS27 sensor
// keep FIFO control reg at zero (ie FIFO off)
// also by default will increment the register address
void initLPS27Hardware(void){
    uint8_t txMsg;
    uint8_t txHeader;

    // setup the CTRL_REG1
    txHeader = WRITE | CTRL_REG1;
    txMsg = DATA_RATE | EN_LPFP | LPFP_CFG | BDU | SIM;
    // send a transmission

    // setup the CTRL_REG2
    txHeader = WRITE | CTRL_REG2;
    txMsg = LOW_NOISE_EN;
    // send a transmission
    
    // sensor also requires ~4.5 ms after power up to initialize

    // could do a "who am I" check as a initialization test
}

// function calculate the pressure given the three sensor addresses
double calcPressure(void)
{
    // little different from their datasheet, but logically I believe this is what they mean
    // yields pressure in hPa
    return (highPressureReg << 16 | lowPressureReg << 8 | xlowPressureReg)/4096.;
}

/* SPI Setup / Transaction Helpers ---------------------------------------*/

// function sets up an SPI transaction to read data from high pressure reg
void initHighPressureTransaction(void)
{
    // build the addressing / header line: R + Pressure out
    uint8_t txHeader = READ | PRESSURE_OUT_H;
    // call on spi transaction, fill in data with the RXNE interrupt handling
}

// store in here in the likely event that the interrupt is serviced elsewhere
void storeHighPressureReading(uint8_t rxne_reading)
{
    highPressureReg = rxne_reading;
}

// function sets up an SPI transaction to read data from lo pressure reg
void initLowPressureTransaction(void)
{
    // build the addressing / header line: R + pressure low
    uint8_t txHeader = READ | PRESSURE_OUT_L;
    // call on spi transaction, fill in data with the RXNE interrupt handling
}

void storeLowPressureReading(uint8_t rxne_reading)
{
    lowPressureReg = rxne_reading;
}

// function sets up an SPI transaction to read data from lo pressure reg
void initXtraLowPressureTransaction(void)
{
    // build the addressing / header line: R + pressure low
    uint8_t txHeader = READ | PRESSURE_OUT_XL;
    // call on spi transaction, fill in data with the RXNE interrupt handling
}

void storeXtraLowPressureReading(uint8_t rxne_reading)
{
    xlowPressureReg = rxne_reading;
}



