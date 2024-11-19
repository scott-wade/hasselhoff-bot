#include "comms.h"
#include "stdlib.h"

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
        //delay(1);
    }
}

void recv_comms()
{
    // Read the packet
    uint8_t* data = malloc(sizeof(comms_payload_t));
    for (int i = 0; i < sizeof(comms_payload_t); i++)
    {
        data[i] = readRX(1);
        //delay(1);
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
        ack_cb(payload);
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