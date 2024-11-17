/* 
Packet structure and type defs for sub-to-remote and remote-to-sub communication
 */

#pragma once

#include "../main.h"
#include "state_machine_sub.h"
#include "state_machine_remote.h"
#include "hardware_stm_spi.h"


// Type definitions
typedef enum {
    DRIVE_CMD, // remote to sub
    LAND_CMD, // remote to sub
    ACK, // sub to remote and remote to sub
    IR_DETECTED, // sub to remote
    IR_LOST, // sub to remote
    LANDED, // sub to remote
} comms_type_t;

typedef struct {
    uint8_t type;
    double throttle_val; // Only non-zero in DRIVE_MSG
    double heading_val; // Only non-zero in DRIVE_MSG
} comms_payload_t;

void send_comms(comms_type_t type, comms_payload_t payload);


// Callbacks
void drive_cmd_cb(comms_payload_t payload);
void land_cmd_cb(comms_payload_t payload);
void ack_cb(comms_payload_t payload);
void ir_detected_cb(comms_payload_t payload);
void ir_lost_cb(comms_payload_t payload);
void landed_cb(comms_payload_t payload);
