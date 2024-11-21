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
#include "stm32f4xx_rcc_mort.h"


/* MACRO definitions----------------------------------------------------------*/
#define ATM_PRESS 1013.25        // atmospheric pressure in hPa

/* Declarations ----------------------------------------------------------*/
static double resultantPressure;
static double currentDepth;

/* Functions. ----------------------------------------------------------*/
// initialization of pressure sensor
void initPressureSensor(void)
{
    // call stuff at the hardware level for initializing
}

// function to kick off getting a pressure sensor reading
void measurePressure(void)
{
    // call on the SPI state machine to kick off the chain of pressure readings
    
}

double getPressure(void)
{
    resultantPressure = calcPressure();
    return resultantPressure;
}

// function to calculate depth in inches based on hPa reading
void calcDepth(void)
{
    currentDepth = (getPressure() - ATM_PRESS) * 0.40146;
}

double getDepth(void)
{
    return currentDepth;
}
