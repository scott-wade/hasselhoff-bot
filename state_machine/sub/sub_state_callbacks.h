
#ifndef STATE_MACHINE_CALLBACK_H
#define STATE_MACHINE_CALLBACK_H

#include "globals.h"
#include "motor_controller.h"
#include "hardware_stm_spi.h"
#include "state_machine_SPI.h"
#include "comms.h"
#include "applications/ir_range.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define WELCOME_DEPTH 6
#define LAND_DEPTH 15
#define DEPTH_TOLERANCE 1
#define MIN_POT_DEPTH 1.0
#define MAX_POT_DEPTH 17.0

// IDLE State callbacks
void any_message_in_idle(void);

// WELCOME State callbacks
void drive_message_in_welcome(void);
void default_in_welcome(void);

// DRIVE State callbacks
void drive_message_in_drive(void);
void land_message_in_drive(void);

// LANDING State callbacks
void land_message_in_land(void);

// Auxiliary functions
void poll_sensors(void);
void reset_message_in_any_state(void);



#ifdef __cplusplus
}
#endif

#endif
