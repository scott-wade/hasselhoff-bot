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
#include "queue.h"
#include "stdio.h"
#include "../applications/packet.h"

// define global subState
sub_t subState = { .state = IDLE, .initialised = 0, 
                .beam_detected = 0, .lr_command_stash = 0, 
                .ds_command_stash = 0
                };

// define global sub state message
uint8_t SUBMARINE_CURRENT_STATUS_MSG;

void init_sub(void){
    /* Initialize sub state machine */ 
    subState.state = IDLE;
    subState.initialised = 0;
    subState.beam_detected = 0;
    subState.lr_command_stash = 0;
    subState.ds_command_stash = 0;
    SUBMARINE_CURRENT_STATUS_MSG = subStateToSubStatusMsg(subState);
}

void init_sub_debugging(sub_states_t testState, uint8_t testBeam){
    /* Initialize sub state machine */ 
    subState.state = testState;
    subState.initialised = 0;
    subState.beam_detected = testBeam;
    subState.lr_command_stash = 0;
    subState.ds_command_stash = 0;
    SUBMARINE_CURRENT_STATUS_MSG = subStateToSubStatusMsg(subState);
}


void event_handler_sub(){
    /* Checks and handles events for sub */
    sub_events_t current_event;
    if(~isempty_simple_queue()){
        current_event = pop_from_simple_queue();
        printf("Processing event of type %d\n", current_event.type);

        switch (current_event.type) {
            case DRIVE_MSG_FB_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case WELCOME: break;
                        case DRIVE:
                            break;
                        case LANDING:
                            break;
                        default: break;
                    }
                break;
            case DRIVE_MSG_LR_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case WELCOME: break;
                        case DRIVE:
                            break;
                        case LANDING:
                            break;
                        default: break;
                    }
                break;
            case DRIVE_MSG_DS_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case WELCOME: break;
                        case DRIVE:
                            break;
                        case LANDING:
                            break;
                        default: break;
                    }
                break;
            case LAND_MSG_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case WELCOME: break;
                        case DRIVE:
                            break;
                        case LANDING:
                            break;
                        default: break;
                    }
                break;
            case RESET_MSG_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case WELCOME: break;
                        case DRIVE:
                            break;
                        case LANDING:
                            break;
                        default: break;
                    }
                break;
            case IR_REQUEST_RECEIVED:
                switch(subState.state)
                    {
                        case IDLE: break; // first packet received
                        case LANDING: break;
                        case DRIVE:
                            break;
                        case WELCOME:
                            break;
                        default: break;
                    }
                break;
            case SENSOR_POLLING_TIMEOUT:
                // queue sensor spi stuff regardless of state
                break;
            default:
                // When undefined event fired, throw an error
                printf(stderr, "Undefined Sub Event");
                break;
        }
    }
    printf("--------------------\n");


}
