/* 
Packet structure and type defs for sub-to-remote and remote-to-sub communication
 */

#pragma once

#include "main.h"
#include "state_machine_sub.h"
#include "state_machine_remote.h"
#include "hardware_stm_spi.h"


// Type definitions
typedef enum {
    DRIVE_CMD, // remote to sub
    LAND_CMD, // remote to sub
    ACK, // sub to remote and remote to sub
    IR_DETECTED, // sub to remote
    IR_RESPONSE, // sub to remote
    IR_LOST, // sub to remote
    LANDED_MSG, // sub to remote
} comms_type_t;

typedef struct {
    uint8_t type;
    uint8_t payload;
} comms_payload_t;


static uint8_t* debug_packet;

void send_comms(comms_type_t type, comms_payload_t* payload);
void recv_comms();
void recv_comms_debug();

void set_debug_packet(uint16_t* packet);
uint8_t* get_debug_packet();

// Callbacks
void drive_cmd_cb(comms_payload_t* payload);
void land_cmd_cb(comms_payload_t* payload);
void ack_cb(comms_payload_t* payload);
void ir_detected_cb(comms_payload_t* payload);
void ir_lost_cb(comms_payload_t* payload);
void landed_cb(comms_payload_t* payload);
