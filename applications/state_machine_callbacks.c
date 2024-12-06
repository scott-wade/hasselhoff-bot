#include "state_machine_callbacks.h"

void idle_callback(void)
{
    // Send acknowledgement
    // Transition to Welcome
    subState.state = WELCOME;
}

void welcome_callback(void)
{
    // Transition to DRIVE
    subState.state = DRIVE;
    // IR Receiver on
}

void throttle_callback(void)
{
    float fb_command = (subState.fb_command-128)/128;
    float lr_command = (subState.lr_command - 128)/128;
    float ds_command = 1 + 16*(subState.ds_command)/255; // HARDCODED max and min depths. TO BE CHANGED

    propulsionControl(fb_command, lr_command, ds_command);
}

void sensor_poll_callback(void)
{
    // Queue SPI transmissions
    // Get other sensor readings
    subState.beam_detected = getSensorTripped();
}


// Drive message throttle callbacks
void throttle_message_in_idle(uint8_t value)
{
    idle_callback();
}
void throttle_message_in_welcome(uint8_t value)
{
    welcome_callback();
}
void throttle_message_in_drive(uint8_t value)
{
    // // Queue corresponding prop controls
    // sub_events_t event;
    // event.type = DRIVE_MSG_FB_RECEIVED;
    // event.value = value;
    //TODO: Add queue
}
void throttle_message_in_land(uint8_t value)
{
    return; // Ignore
}

// Drive message steering callbacks
void steering_message_in_idle(uint8_t value)
{
    idle_callback();
}
void steering_message_in_welcome(uint8_t value)
{
    welcome_callback();
}
void steering_message_in_drive(uint8_t value)
{
    // Queue corresponding prop controls
    // sub_events_t event;
    // event.type = DRIVE_MSG_LR_RECEIVED;
    // event.value = value;
    //TODO: Add queue
    subState.lr_command = value;

}
void steering_message_in_land(uint8_t value)
{
    return; // Ignore
}

// Drive message dive/surface callback
void dive_message_in_idle(uint8_t value)
{
    idle_callback();
}
void dive_message_in_welcome(uint8_t value)
{
    welcome_callback();
}
void dive_message_in_drive(uint8_t value)
{
    // Queue corresponding prop controls
    // sub_events_t event;
    // event.type = DRIVE_MSG_DS_RECEIVED;
    // event.value = value;
    //TODO: Add queue
    subState.ds_command = value;
}
void dive_message_in_land(uint8_t value)
{
    return;
}

// Land message received
void land_message_in_idle(void)
{
    idle_callback();
}
void land_message_in_welcome(void)
{
    return; // Ignore
}
void land_message_in_drive(void)
{
    // Transition to land state
    // subState.state = LANDING;
    // // Queue corresponding prop controls
    // sub_events_t event;
    // event.type = DRIVE_MSG_DS_RECEIVED;
    // event.value = value;
    //TODO: Add queue
    // insert_to_simple_queue(event);

}
void land_message_in_land(void)
{
    return;
}


// Landing finished
void landing_finished_in_idle(void)
{
    return; // Ignore
}
void landing_finished_in_welcome(void)
{
    return; // Ignore
}
void landing_finished_in_drive(void)
{
    return; // Ignore
}
void landing_finished_in_land(void)
{
    // subState.state = WELCOME;
    // // Queue corresponding prop controls
    // sub_events_t event;
    // event.type = DRIVE_MSG_DS_RECEIVED;
    // event.value = value;
    //TODO: Add queue
    // insert_to_simple_queue(event);
}

// IR Status Request
void IR_request_message_in_idle(void)
{
    welcome_callback();
}
void IR_request_message_in_welcome(void)
{
    return; // Ignore
}
void IR_request_message_in_drive(void)
{
    return; // Ignore  
}
void IR_request_message_in_land(void)
{
    return; // Ignore
}

// Reset packet
void reset_message_in_idle(void)
{
    idle_callback();
}
void reset_message_in_welcome(void)
{
    return; // Ignore
}
void reset_message_in_drive(void)
{
    // Transition to welcome
    subState.state = WELCOME;
}
void reset_message_in_land(void)
{
    // Transition to welcome
    subState.state = WELCOME;
}

// Sensor polling timeout
void sensor_polling_in_idle(void)
{
    return; // Ignore
}
void sensor_polling_in_welcome(void)
{
    sensor_poll_callback();
}
void sensor_polling_in_drive(void)
{
    sensor_poll_callback();
}
void sensor_polling_in_land(void)
{
    sensor_poll_callback();
}
