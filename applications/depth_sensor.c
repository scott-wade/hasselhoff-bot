/**
 ******************************************************************************
 * @file    hardware_lps_spi.c
 * @author  nathanz@andrew.cmu.edu
 * @version 1.0
 * @date    November 2024
 * @brief   LPS27HHW SPI
 ******************************************************************************
 */

#include "depth_sensor.h"
#include "hardware/hardware_lps_spi.h"
#include "state_machine_SPI.h"
#include "stm32f4xx_rcc_mort.h"


/* MACRO definitions----------------------------------------------------------*/
#define ATM_PRESS 1013.25        // atmospheric pressure in hPa
#define DEPTH_SENSOR_ID 2

/* Declarations ----------------------------------------------------------*/
static double resultantPressure;
static double currentDepth;
static uint8_t readingBuffer;

static uint8_t highPressureReg;
static uint8_t lowPressureReg;
static uint8_t xlowPressureReg;

/* Functions. ----------------------------------------------------------*/
// initialization of pressure sensor
void initPressureSensor(void)
{
    // setup the GPIO pins of the nucleo for SPI ?
    // ^probably better managed by the SPI initialization

    // use hardware level functions to generate messages for intializing
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initLPS27HardwareMsg1(), &readingBuffer);
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initLPS27HardwareMsg2(), &readingBuffer);
    // issue a reading request for the who am I register as a final check
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, whoAmILPS27Msg(), &readingBuffer);
}

// call this function from the spi state machine level once the initializing receipts are complete
// verifies the who am I reading matches appropriate flag
void validateSensorInitMsg(void)
{
    int sensorCheck;
    sensorCheck = verifyWhoAmI(readingBuffer);
    if(sensorCheck != 1)
    {
        //fprintf(stderr, "Depth sensor 'who am I' reading mismatch");
    }
}

// function to kick off getting a pressure sensor reading
void measurePressure(void)
{
    // call from the SPI state machine to kick off the chain of pressure readings
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initHighPressureTransaction(), &highPressureReg);
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initLowPressureTransaction(), &lowPressureReg);
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initXtraLowPressureTransaction(), &xlowPressureReg);
}

double getPressure(void)
{
    resultantPressure = calcPressure(highPressureReg, lowPressureReg, xlowPressureReg);
    return resultantPressure;
}

// function to calculate depth in inches based on hPa reading
// this should get called upon completion of the SPI state machine when transitioning out of the final pressure reading
// that will ensure that whenever getDepth is called on, it will be the freshest reading
void calcDepth(void)
{
    currentDepth = (getPressure() - ATM_PRESS) * 0.40146;
}

double getDepth(void)
{
    return currentDepth;
}
