#include "state_machine_callbacks.h"
#include "state_machine_sub.h"
#include "motor_controller.h"
#include "depth_sensor.h"
#include "ir_range.h"
#include <math.h>

#define WELCOME_DEPTH 6
#define LAND_DEPTH 15
#define DEPTH_TOLERANCE 1
#define MIN_POT_DEPTH 4.0
#define MAX_POT_DEPTH 10.0

void any_message_in_idle(void)
{
    // Transition to WELCOME
    subState.state = WELCOME;
}

void drive_message_in_welcome(void)
{
    // Transition to DRIVE
    subState.state = DRIVE;
}

void default_in_welcome(void)
{
    // Execute Welcome Trajectory
    welcomeTrajectory();
}

void drive_message_in_drive(void)
{
    float ds_command = MIN_POT_DEPTH + (subState.ds_command/255.0)*(MAX_POT_DEPTH - MIN_POT_DEPTH);
    float lr_command = (subState.lr_command-128.0)/128.0;
    float fb_command = (subState.fb_command-128.0)/128.0;
    
    // Get current depth
    float current_depth = subState.current_depth;
    int sign = (current_depth > WELCOME_DEPTH) - (current_depth < WELCOME_DEPTH);
    float z_input = sign*0.5*(fabs(current_depth - ds_command) > DEPTH_TOLERANCE);

    planarControl(fb_command, lr_command);
    depthControl(z_input);
}

void land_message_in_drive(void)
{
    // Halt all motors
    planarControl(0.0, 0.0);
    depthControl(0.0);

    // Transition to LAND state
    subState.state = LANDING;
}

void reset_message_in_any_state(void)
{
    int sign;
    float z_input;

    // Halt all motors
    planarControl(0.0, 0.0);
    depthControl(0.0);

    // Raise to Welcome Depth (BLOCKING CODE)
    float current_depth = subState.current_depth;

    while (fabs(current_depth - WELCOME_DEPTH) > DEPTH_TOLERANCE) {
        int sign = (current_depth > WELCOME_DEPTH) - (current_depth < WELCOME_DEPTH);
        float z_input = sign*0.5*(fabs(current_depth - WELCOME_DEPTH) > DEPTH_TOLERANCE);

        depthControl(z_input);

        current_depth = updateDepth(); // UPDATE depth sensor reading
    }

    // Transition to WELCOME
    subState.state = WELCOME;
}

int land_message_in_land(void)
{
    float current_depth = subState.current_depth;
    if (fabs(current_depth - WELCOME_DEPTH) <= DEPTH_TOLERANCE) {
        return 1;
    } else {
        depthControl(-0.5);
        return 0;
    }
}

void poll_sensors(void)
{
    // Read Depth Sensor
    subState.current_depth = updateDepth();

    // IR Beam status
    subState.beam_status = getSensorTripped();
}
