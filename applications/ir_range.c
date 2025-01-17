#include "ir_range.h"

void initIRSensor(uint8_t analog) // What does this Analog argument do?
{
    initGPIOasMode(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM, 
                    MODE_IN, OD_PUPD, PUPD_UP, 0, 0);
}

uint8_t getSensorTripped(void)
{
    // Read the digital value from the sensor
    // Return the digital value
    int beam_status = readGPIOinput(IR_RANGE_PORT_NUM, IR_RANGE_PIN_NUM);
    return beam_status;
}