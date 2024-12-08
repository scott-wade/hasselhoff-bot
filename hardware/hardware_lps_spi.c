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
#define WHO_AM_I_REG            0x0F
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
#define WRITE                   0b0 << 7
// CTRL REG1 FLAGS
#define DATA_RATE               0b101 << 4  // 75 Hz max data rate (though this gets halved at min regardless)
#define EN_LPFP                 0b0 << 3    // even with filters off, can still get really low noise
#define LPFP_CFG                0b0 << 2
#define BDU                     0b0 << 1
#define SIM                     0b0
// CTRL REG2 FLAGS
#define LOW_NOISE_EN            0b1 << 1
#define ID_FLAG                 0b10110011  // flag that is stored at the "who am I" register

#define DEPTH_TX_BUFFER         (uint8_t)0  // just a buffer of zeroes to fill out transmission messages during read requests

/* Declarations ----------------------------------------------------------*/


/* Functions. ----------------------------------------------------------*/
// function initializes the LPS27 sensor
// keep FIFO control reg at zero (ie FIFO off)
// also by default will increment the register address (but let's not worry about the multi byte messages)
uint16_t initLPS27HardwareMsg1(void){
    uint8_t txMsg;
    uint8_t txHeader;
    uint16_t overallMsg;
    // setup the CTRL_REG1
    txHeader = WRITE | CTRL_REG1;
    txMsg = DATA_RATE | EN_LPFP | LPFP_CFG | BDU | SIM;
    // send a transmission
    overallMsg = txHeader << 8 | txMsg;

    return overallMsg;
}

uint16_t initLPS27HardwareMsg2(void){
    uint8_t txMsg;
    uint8_t txHeader;
    uint16_t overallMsg;    
    // setup the CTRL_REG2
    txHeader = WRITE | CTRL_REG2;
    txMsg = LOW_NOISE_EN;
    // send a transmission
    overallMsg = txHeader << 8 | txMsg;

    return overallMsg;
}

// sensor also requires ~4.5 ms after power up to initialize
// could do a "who am I" check as a initialization test
uint16_t whoAmILPS27Msg(void) {
    uint8_t txMsg;
    uint8_t txHeader;
    uint16_t overallMsg; 

    // setup message to the who am I register
    txHeader = READ | WHO_AM_I_REG;
    txMsg = DEPTH_TX_BUFFER;    
    overallMsg = txHeader << 8 | txMsg;
    return overallMsg;
}

int verifyWhoAmI(uint8_t checkReading)
{
    return checkReading == ID_FLAG;
}

// function calculate the pressure given the three sensor addresses
double calcPressure(uint32_t highPressureVal, uint32_t lowPressureVal, uint32_t xLowPressureVal)
{
    // little different from their datasheet, but logically I believe this is what they mean
    int32_t combineSignedPressure = (highPressureVal << 16 | lowPressureVal << 8 | xLowPressureVal);
    if (combineSignedPressure & 0x800000) { // Check the 24th bit
        printf("neg pressure ?");
        combineSignedPressure |= 0xFF000000; // Extend the sign bit
    }    
    // return scaled pressure in hPa
    return combineSignedPressure/4096.;
    // return (double)(highPressureVal << 16 | lowPressureVal << 8 | xLowPressureVal)/4096.;
}

/* SPI Setup / Transaction Helpers ---------------------------------------*/

// function sets up an SPI transaction to read data from high pressure reg
uint16_t initHighPressureTransaction(void)
{
    uint16_t overallMsg;
    // build the addressing / header line: R + Pressure out
    uint8_t txHeader = READ | PRESSURE_OUT_H;
    uint8_t txMsg = DEPTH_TX_BUFFER;
    
    overallMsg = txHeader << 8 | txMsg;
    return overallMsg;
}


// function sets up an SPI transaction to read data from lo pressure reg
uint16_t initLowPressureTransaction(void)
{
    uint16_t overallMsg;
    // build the addressing / header line: R + pressure low
    uint8_t txHeader = READ | PRESSURE_OUT_L;
    uint8_t txMsg = DEPTH_TX_BUFFER;

    overallMsg = txHeader << 8 | txMsg;
    return overallMsg;
}

// function sets up an SPI transaction to read data from lo pressure reg
uint16_t initXtraLowPressureTransaction(void)
{
    uint16_t overallMsg;
    // build the addressing / header line: R + pressure low
    uint8_t txHeader = READ | PRESSURE_OUT_XL;
    uint8_t txMsg = DEPTH_TX_BUFFER;

    overallMsg = txHeader << 8 | txMsg;
    return overallMsg;
    // call on spi transaction, fill in data with the RXNE interrupt handling
}


