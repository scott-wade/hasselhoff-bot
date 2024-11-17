#include "ir_range.h"

void initSensor(bool analog)
{
    uint8_T MODE;
    if (analog)
    {
        MODE = 2; // Analog mode
    }
    else
    {
        MODE = 0; // Digital mode
    }
    initGPIOasMode(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM, MODE);
}

/* Deprecated */
// double getDistanceAnalog(void)
// {
//     // Read the analog value from the sensor
//     // Convert the analog value to distance
//     // Return the distance
//     uint32_t value = readADC(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM);
//     return value * ANALOG_TO_DISTANCE;
// }

uint8_t getSensorTripped(void)
{
    // Read the digital value from the sensor
    // Return the digital value
    return readGpioInput(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM);
}