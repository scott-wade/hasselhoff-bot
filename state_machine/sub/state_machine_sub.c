/**
  ******************************************************************************
  * @file    state_machine_sub.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for Hasselhoff Submarine
  ******************************************************************************
  */


#include "stdio.h"
#include "state_machine_sub.h"
#include "globals.h"
#include "motor_controller.h"
#include "queue.h"
#include "ir_range.h"
#include "sub_state_callbacks.h"
#include "packet.h"

// define global subState
sub_t subState = { .state = IDLE, .initialised = 0, 
                .beam_status = 0, .current_depth = 0.0, .land_status = 0, .reset_status = 0,
                .lr_command = 0, .ds_command = 0, .fb_command = 0
                };

void init_sub(void){
    /* Initialize all Hardware */
    initMotorHardware();
    // TODO : initialize Depth Sensor
    initIRSensor(0);

    /* Initialize State Machine */ 
    subState.state = IDLE;
    subState.initialised = 0;
    subState.beam_status = 0;
    subState.current_depth = 0.0;
    subState.land_status = 0;
    subState.reset_status = 0;
    subState.lr_command = 0;
    subState.ds_command = 0;
    subState.fb_command = 0;

    init_simple_queue(0);
}

// TODO : DELETE
void init_sub_debugging(sub_states_t testState, uint8_t testBeam){
    /* Initialize sub state machine */ 
    subState.state = testState;
    subState.initialised = 0;
    subState.beam_status = testBeam;
    subState.current_depth = 0.0;
    subState.land_status = 0;
    subState.reset_status = 0;
    subState.lr_command = 0;
    subState.ds_command = 0;
    subState.fb_command = 0;
}


void event_handler_sub(){
    /* Checks and handles events for sub */
    sub_events_t current_event;
    printf("simpleQ size is %u \n", simpleQ.size);
    if(!isempty_simple_queue()){
        current_event = pop_from_simple_queue();
        printf("Processing event of type %d\n", current_event.type);

        switch(subState.state) {
            case IDLE: {
                switch(current_event.type) {
                    case SENSOR_POLLING_TIMEOUT: {
                        poll_sensors();
                        break;
                    }
                    default: {
                        any_message_in_idle();
                        break;
                    }
                }
                break;
            }
            case WELCOME: {
                switch(current_event.type) {
                    case DRIVE_MSG_DS_RECEIVED:
                    case DRIVE_MSG_LR_RECEIVED:
                    case DRIVE_MSG_FB_RECEIVED: {
                        drive_message_in_welcome();
                        break;
                    }
                    case SENSOR_POLLING_TIMEOUT: {
                        poll_sensors();
                        break;
                    }
                    default: {
                        default_in_welcome();
                        break;
                    }
                }
                break;
            }
            case DRIVE: {
                switch(current_event.type) {
                    case DRIVE_MSG_DS_RECEIVED: {
                        subState.ds_command = current_event.data;
                        break;
                    }
                    case DRIVE_MSG_LR_RECEIVED: {
                        subState.lr_command = current_event.data;
                        break;
                    }
                    case DRIVE_MSG_FB_RECEIVED: {
                        subState.fb_command = current_event.data;
                        drive_message_in_drive();
                        break;
                    }
                    case LAND_MSG_RECEIVED: {
                        land_message_in_drive();
                        break;
                    }
                    case RESET_MSG_RECEIVED: {
                        reset_message_in_any_state();
                        break;
                    }
                    case SENSOR_POLLING_TIMEOUT: {
                        poll_sensors();
                        break;
                    }
                }
                break;
            }
            case LANDING: {
                switch(current_event.type) {
                    case RESET_MSG_RECEIVED: {
                        reset_message_in_any_state();
                        break;
                    }
                    case SENSOR_POLLING_TIMEOUT: {
                        poll_sensors();
                        land_message_in_land();
                        break;
                    }
                    default: {
                        land_message_in_land();
                        break;
                    }
                }
                break;
            }
            case RESET: {
                switch (current_event.type)
                {                
                    default:
                        reset_message_in_any_state();
                        break;
                }
                break;
            }
            default: {
                // When undefined event fired, throw an error
                printf(stderr, "Undefined Sub Event");
                break;
            }
        }
        printf("--------------------\n");
    }
}
