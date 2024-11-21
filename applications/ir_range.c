#include "ir_range.h"

void initSensor(uint8_t analog)
{
    initGPIOasMode(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM, 
                    MODE_IN, OD_PUPD, PUPD_UP, 0, 0);
}

uint8_t getSensorTripped(void)
{
    // Read the digital value from the sensor
    // Return the digital value
    return readGPIOinput(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM);
}