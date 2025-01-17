#include "sub_state_callbacks.h"
#include "state_machine_sub.h"
#include "motor_controller.h"
#include "depth_sensor.h"
#include "ir_range.h"
#include <math.h>
#include "packet.h"

void any_message_in_idle(void)
{
    // Transition to WELCOME
    subState.state = WELCOME;
    // reload the status packet to send back to the remote
    loadTXPacket(subState);
    // queue up first set of pressure readings to set the atmospheric pressure variable (used in calculating depth)
    initPressure();
}

void drive_message_in_welcome(void)
{
    // Transition to DRIVE
    subState.state = DRIVE;
    // reload the status packet to send back to the remote
    loadTXPacket(subState);
}

void default_in_welcome(void)
{
    // Execute Welcome Trajectory
    welcomeTrajectory();
}

void drive_message_in_drive(void)
{
    float ds_command = MIN_POT_DEPTH + (subState.ds_command/255.0)*(MAX_POT_DEPTH - MIN_POT_DEPTH);
    float lr_command = (subState.lr_command-5.0)/5.0;
    float fb_command = (subState.fb_command-5.0)/5.0;
    
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
    // reload the status packet to send back to the remote
    loadTXPacket(subState);
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

    if (fabs(current_depth - WELCOME_DEPTH) > DEPTH_TOLERANCE) {
        int sign = (current_depth > WELCOME_DEPTH) - (current_depth < WELCOME_DEPTH);
        float z_input = sign*0.5*(fabs(current_depth - WELCOME_DEPTH) > DEPTH_TOLERANCE);

        depthControl(z_input);

        current_depth = getDepth(); // UPDATE depth sensor reading

        subState.reset_status = 0;

    } else {

        // Halt all motors
        planarControl(0.0, 0.0);
        depthControl(0.0);

        // Transition to WELCOME
        subState.state = WELCOME;
        clear_simple_queue();

        subState.reset_status = 1;
    }
    // reload the status packet to send back to the remote
    loadTXPacket(subState);
}

void land_message_in_land(void)
{
    float current_depth = subState.current_depth;
    if (fabs(current_depth - WELCOME_DEPTH) <= DEPTH_TOLERANCE) {
        subState.land_status = 1; // Update internal state to match landing status
    } else {
        depthControl(-0.5);
        subState.land_status = 0;
    }
    // reload the status packet to send back to the remote
    loadTXPacket(subState);
}

void poll_sensors(void)
{
    // Queue up depth sensor readings to update pressure readings in memory
    measurePressure();
    // Update depth calculation based on pressure in memory
    subState.current_depth = getDepth();

    // IR Beam status
    subState.beam_status = getSensorTripped();

    // reload the status packet to send back to the remote
    loadTXPacket(subState);
}
