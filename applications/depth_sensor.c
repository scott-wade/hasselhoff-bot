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
#define DEPTH_SENSOR_ID 0

/* Declarations ----------------------------------------------------------*/
static double resultantPressure;
static double atmPressure;
static double currentDepth;
static uint8_t readingBuffer;

static uint8_t highPressureReg;
static uint8_t lowPressureReg;
static uint8_t xlowPressureReg;
static uint8_t firstReading = 1;

/* Functions. ----------------------------------------------------------*/
// initialize a value to be used as atmospheric pressure / zero depth
void initPressure(void)
{
    // pin initialization handled by SPI intialization
    // call for a pressure reading (dummy proofs taking a pressure reading early)
    measurePressure();
}

void initPressureSensorSettings(void)
{
    // use hardware level functions to generate messages for intializing
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initLPS27HardwareMsg1(), &readingBuffer);
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, initLPS27HardwareMsg2(), &readingBuffer);
    // issue a reading request for the who am I register as a final check
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, whoAmILPS27Msg(), &readingBuffer);
}

void extraWhoAmICheck(void) {
    // issue a reading request for the who am I register as a final check
    requestSpiTransmit(SENSOR_PARENT, DEPTH_SENSOR_ID, whoAmILPS27Msg(), &readingBuffer);    
}

// call this function from the spi state machine level once the initializing receipts are complete
// verifies the who am I reading matches appropriate flag
void validateSensorInitMsg(void)
{
    int sensorCheck = 0;
    sensorCheck = verifyWhoAmI(readingBuffer);
    if(sensorCheck != 1)
    {

        printf("Depth sensor 'who am I' reading mismatch \n");
    }
    else
    {
        printf("depth hello world\n");
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
    // when we take our first pressure reading and calculate a result, treat this as our atmospheric 0 pressure point
    if(firstReading)
    {
        atmPressure = resultantPressure;
        firstReading = 0;
    }
    return resultantPressure;
}

// function to calculate depth in inches based on hPa reading
void calcDepth(void)
{
    currentDepth = (getPressure() - atmPressure) * 0.401869;
}

double getDepth(void)
{
    calcDepth();
    return currentDepth;
}
