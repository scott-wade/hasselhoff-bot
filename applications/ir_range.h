#pragma once

#include "pin_assignment.h"
#include "hardware_stm_gpio.h"
#include "../main.h"

// #define ANALOG_TO_DISTANCE 1.0

void initIRSensor(uint8_t analog);
// double getDistanceAnalog(void);
uint8_t getSensorTripped(void);