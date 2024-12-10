/**
  ******************************************************************************
  * @file    packet.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   Helper methods for packet handling
  ******************************************************************************
  */


#include "hardware_stm_spi.h"
#include "state_machine_sub.h"
#include "globals.h"
#include "stdio.h"
#include "packet.h"

uint8_t subStateToSubStatusMsg(sub_t state_struct){
    /* Convert a sub_states_t struct to an 8-bit data packet
        typedef struct {
    sub_states_t state;
    uint8_t initialised;
    uint8_t beam_status;
    uint8_t land_status;
    uint8_t lr_command;
    uint8_t ds_command;
    uint8_t fb_command;
} sub_t;
    */

    uint8_t sub_status_msg = 0;

    // Bit 0 is IR status
     sub_status_msg = sub_status_msg | (uint8_t)(state_struct.beam_status << 7);

    // Bit 1 is Land Status
    sub_status_msg = sub_status_msg | (uint8_t)(state_struct.land_status << 6);

    // Bit 2 is Reset Status
    sub_status_msg = sub_status_msg | (uint8_t)(state_struct.reset_status << 5);


    // bits [3-4] are sub state
    uint8_t substate_mask = (uint8_t)(0b00011000);
    switch (state_struct.state){
        case IDLE: 
            sub_status_msg = sub_status_msg | (uint8_t)0b00000000;
            break;
        case WELCOME:
            sub_status_msg = sub_status_msg | (uint8_t)0b00001000;
            break;
        case DRIVE:
            sub_status_msg = sub_status_msg | (uint8_t)0b00010000;
            break;
        case LANDING:
            sub_status_msg = sub_status_msg | (uint8_t)0b00011000;
            break;
    }

    printf("Generated message: %u \n ", sub_status_msg);
    
    return sub_status_msg;
}

void loadTXPacket(sub_t state_struct){
    uint8_t packet = subStateToSubStatusMsg(state_struct);
    writeTX(1, packet);
}
