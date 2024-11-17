/* 
Packet structure and type defs for sub-to-remote and remote-to-sub communication
 */

#pragma once

#include "../main.h"


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