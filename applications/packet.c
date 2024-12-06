/**
  ******************************************************************************
  * @file    packet.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   Helper methods for packet handling
  ******************************************************************************
  */


#include "state_machine_sub.h"
#include "globals.h"
#include "stdio.h"
#include "packet.h"

uint8_t subStateToSubStatusMsg(sub_t state_struct){
    /* Convert a sub_states_t struct to an 8-bit data packet
        typedef struct {
    sub_states_t state;
    uint8_t initialised;
    uint8_t beam_detected;
    uint8_t lr_command;
    uint8_t ds_command;
    uint8_t fb_command;
} sub_t;
    */

    uint8_t sub_status_msg = 0;

    // first bit is IR status
    if (state_struct.beam_detected > 0){
        sub_status_msg = sub_status_msg | (uint8_t)(0b1 << 7);
    }else{
        sub_status_msg = sub_status_msg & ~(uint8_t)(0b1 << 7);
    }

    // bits [1-2] are sub state
    uint8_t substate_mask = (uint8_t)(0b01100000);
    sub_status_msg = sub_status_msg | (((uint8_t)state_struct.state) & substate_mask);

    printf("Generated message: %u \n ", sub_status_msg);
    
    return sub_status_msg;
}

