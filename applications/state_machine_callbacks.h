
#ifndef STATE_MACHINE_CALLBACK_H
#define STATE_MACHINE_CALLBACK_H

#include "globals.h"

#ifdef __cplusplus
 extern "C" {
#endif

// Drive message throttle callbacks
void throttle_message_in_idle(uint16_t value);
void throttle_message_in_welcome(uint16_t value);
void throttle_message_in_drive(uint16_t value);
void throttle_message_in_land(uint16_t value);

// Drive message steering callbacks
void steering_message_in_idle(uint16_t value);
void steering_message_in_welcome(uint16_t value);
void steering_message_in_drive(uint16_t value);
void steering_message_in_land(uint16_t value);

// Drive message dive/surface callback
void dive_message_in_idle(uint16_t value);
void dive_message_in_welcome(uint16_t value);
void dive_message_in_drive(uint16_t value);
void dive_message_in_land(uint16_t value);

// Land message received
void land_message_in_idle(void);
void land_message_in_welcome(void);
void land_message_in_drive(void);
void land_message_in_land(void);


// Landing finished
void landing_finished_in_idle(void);
void landing_finished_in_welcome(void);
void landing_finished_in_drive(void);
void landing_finished_in_land(void);

// IR Status Request
void IR_request_message_in_idle(void);
void IR_request_message_in_welcome(void);
void IR_request_message_in_drive(void);
void IR_request_message_in_land(void);

// Reset packet
void reset_message_in_idle(void);
void reset_message_in_welcome(void);
void reset_message_in_drive(void);
void reset_message_in_land(void);

// Sensor polling timeout
void sensor_polling_in_idle(void);
void sensor_polling_in_welcome(void);
void sensor_polling_in_drive(void);
void sensor_polling_in_land(void);

#ifdef __cplusplus
}
#endif

#endif