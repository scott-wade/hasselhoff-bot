#include "comms.h"
#include "../main.h"
#include "queue.h"
#include "state_machine_sub.h"
// #include "stdlib.h"


sub_t subState = { .state = INITIALISING, .initialised = 0, .beam_detected = 0};

void send_comms(comms_type_t type, comms_payload_t* payload)
{
    // Send the packet
    payload->type = type;
    uint8_t* payload_ = (uint8_t*) &payload;
    // Send the payload
    for (int i = 0; i < sizeof(comms_payload_t); i++)
    {
        uint16_t p = 0;
        uint8_t byte1 = payload_[i];
        uint8_t byte2 = 0;
        if (i <= sizeof(comms_payload_t))
        {
            byte2 = payload_[i + 1];
        }
        p = byte1 << 8 | byte2;
        writeTX(1, p);
        // delay(1);
    }
}

void recv_comms(simple_queue_t* simpleQ)
{
    // Read the packet
    uint8_t* data = malloc(sizeof(comms_payload_t));
    for (int i = 0; i < sizeof(comms_payload_t); i++)
    {
        data[i] = readRX(1);
        // delay(1);
    }
    // Call the appropriate callback
    comms_payload_t* payload = (comms_payload_t*) data;
    switch (payload->type)
    {
    case DRIVE_CMD:
        drive_cmd_cb(payload);
        break;
    case LAND_CMD:
        land_cmd_cb(payload);
        break;
    case ACK:
        ack_cb(simpleQ, payload);
        break;
    case IR_DETECTED:
        ir_detected_cb(payload);
        break;
    case IR_LOST:
        ir_lost_cb(payload);
        break;
    case LANDED_MSG:
        landed_cb(payload);
        break;
    default:
        break;
    }
}

void set_debug_packet(comms_payload_t* packet)
{
    debug_packet = (uint8_t*) packet;
}

void recv_comms_debug(simple_queue_t* simpleQ)
{
    // Call the appropriate callback
    comms_payload_t* payload = (comms_payload_t*) debug_packet;
    switch (payload->type)
    {
    case DRIVE_CMD:
        drive_cmd_cb(payload);
        break;
    case LAND_CMD:
        land_cmd_cb(payload);
        break;
    case ACK:
        ack_cb(simpleQ, payload);
        break;
    case IR_DETECTED:
        ir_detected_cb(payload);
        break;
    case IR_LOST:
        ir_lost_cb(payload);
        break;
    case LANDED_MSG:
        landed_cb(payload);
        break;
    default:
        break;
    }
}

#ifdef SUB

// Callbacks
void drive_cmd_cb(comms_payload_t* payload)
{
    printf("DRIVING\n");
    // Set the motor speeds
    // Set the motor directions
    return;
}
void land_cmd_cb(comms_payload_t* payload)
{
    if (subState.beam_detected)
    {
        printf("LANDING SEQUENCE\n");
    } else {
        printf("NO LANDING TARGET\n");
    }
    // Run the landing sequence
    // May require a controller to regulate descent etc
}
void ack_cb(simple_queue_t* simpleQ, comms_payload_t* payload)
{
    printf("ACKNOWLEDGED\n");
    sub_events_t event = INITIALISED;
    insert_to_simple_queue(simpleQ, event);
    return;
}

void ir_detected_cb(comms_payload_t* payload)
{
    // fire BEAM_DETECTED event
    // remote only
    return;
}
void ir_lost_cb(comms_payload_t* payload)
{
    // fire BEAM_LOST event
    // remote only
    return;
}
void landed_cb(comms_payload_t* payload)
{
    // fire LANDED event
    // remote only
}

#endif

#ifdef REMOTE
void drive_cmd_cb(comms_payload_t* payload)
{
    // Set the motor speeds
    // Set the motor directions
    return;
}
void land_cmd_cb(comms_payload_t* payload)
{
    // Run the landing sequence
    // May require a controller to regulate descent etc
}
void ack_cb(comms_payload_t* payload)
{
    // fire INITIALISED event
    return;
}
void ir_detected_cb(comms_payload_t* payload)
{
    // fire BEAM_DETECTED event
    return;
}
void ir_lost_cb(comms_payload_t* payload)
{
    // fire BEAM_LOST event
    return;
}
void landed_cb(comms_payload_t* payload)
{
    // fire LANDED event
}
#endif