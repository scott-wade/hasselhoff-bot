#pragma once

#include "pin_assignment.h"
#include "../main.h"

// #define ANALOG_TO_DISTANCE 1.0

void initSensor(bool analog);
// double getDistanceAnalog(void);
uint8_t getSensorTripped(void);