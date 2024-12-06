/**
  ******************************************************************************
  * @file    state_machine_sub.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for Hasselhoff Submarine
  ******************************************************************************
  */


#include "state_machine_sub.h"
#include "globals.h"
#include "../applications/state_machine_callbacks.h"
#include "stdio.h"
#include "../applications/packet.h"

// define global subState
sub_t subState = { .state = IDLE, .initialised = 0, 
                .beam_detected = 0, .lr_command = 0, 
                .ds_command = 0, .fb_command = 0
                };

// define global sub state message
uint8_t SUBMARINE_CURRENT_STATUS_MSG;

void init_sub(void){
    /* Initialize sub state machine */ 
    subState.state = IDLE;
    subState.initialised = 0;
    subState.beam_detected = 0;
    subState.lr_command = 0;
    subState.ds_command = 0;
    subState.fb_command = 0;
    SUBMARINE_CURRENT_STATUS_MSG = subStateToSubStatusMsg(subState);
}

void init_sub_debugging(sub_states_t testState, uint8_t testBeam){
    /* Initialize sub state machine */ 
    subState.state = testState;
    subState.initialised = 0;
    subState.beam_detected = testBeam;
    subState.lr_command = 0;
    subState.ds_command = 0;
    subState.fb_command = 0;
    SUBMARINE_CURRENT_STATUS_MSG = subStateToSubStatusMsg(subState);
}


void event_handler_sub(){
    /* Checks and handles events for sub */
    
    // Get current event
    sub_events_t current_event = simpleQ.events[simpleQ.process_indx];
    printf("Processing event %d\n", simpleQ.process_indx);
    simpleQ.process_indx++;
    simpleQ.size--;
    if (simpleQ.process_indx == MAX_ELEMENTS)
    {
        simpleQ.process_indx = 0;
    }
    printf("Event is: %d\n", current_event.type);

    switch(subState.state) {
        case IDLE: {
            switch(current_event.type) {
                case SENSOR_POLLING_TIMEOUT: {
                    // TODO
                    break;
                }
                default: {
                    idle_callback();
                    break;
                }
            }
        }
        case WELCOME: {
            switch(current_event.type) {
                case DRIVE_MSG_DS_RECEIVED:
                case DRIVE_MSG_LR_RECEIVED:
                case DRIVE_MSG_FB_RECEIVED: {
                    welcome_callback();
                    break;
                }
                case SENSOR_POLLING_TIMEOUT: {
                    // TODO
                    break;
                }
                default: break;
            }
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
                    throttle_callback();
                    break;
                }
                case LAND_MSG_RECEIVED: {
                    land_message_in_drive();
                    break;
                }
                case RESET_MSG_RECEIVED: {
                    reset_message_in_drive();
                    break;
                }
                case SENSOR_POLLING_TIMEOUT: {
                    // TODO
                    break;
                }
            }
        }
        case LANDING: {
            switch(current_event.type) {
                case LAND_MSG_RECEIVED: {
                    land_message_in_land();
                    break;
                }
                case RESET_MSG_RECEIVED: {
                    reset_message_in_land();
                    break;
                }
                case SENSOR_POLLING_TIMEOUT: {
                    // TODO
                    break;
                }
            }
        }
        default: {
            // When undefined event fired, throw an error
            printf(stderr, "Undefined Sub Event");
            break;
        }
    }
    printf("--------------------\n");


}
