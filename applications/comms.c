#include "comms.h"

void send_comms(comms_type_t type, comms_payload_t payload)
{
    // Send the packet
    payload.type = type;
    // Send the payload
    for (int i = 0; i < sizeof(payload); i++)
    {
        writeTX(SPI1, payload[i]);
        delay(1);
    }
}

void recv_comms()
{
    // Read the packet
    comms_payload_t payload;
    for (int i = 0; i < sizeof(payload); i++)
    {
        payload[i] = readRX(SPI1);
        delay(1);
    }
    // Call the appropriate callback
    switch (payload.type)
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
    case LANDED:
        landed_cb(payload);
        break;
    default:
        break;
    }
}

#ifdef SUB

// Callbacks
void drive_cmd_cb(comms_payload_t payload)
{
    // Set the motor speeds
    // Set the motor directions
    return;
}
void land_cmd_cb(comms_payload_t payload)
{
    // Run the landing sequence
    // May require a controller to regulate descent etc
}
void ack_cb(comms_payload_t payload)
{
    // fire INITIALISED event
    return;
}
void ir_detected_cb(comms_payload_t payload)
{
    // fire BEAM_DETECTED event
    return;
}
void ir_lost_cb(comms_payload_t payload)
{
    // fire BEAM_LOST event
    return;
}
void landed_cb(comms_payload_t payload)
{
    // fire LANDED event
}

#endif

#ifdef REMOTE
void drive_cmd_cb(comms_payload_t payload)
{
    // Set the motor speeds
    // Set the motor directions
    return;
}
void land_cmd_cb(comms_payload_t payload)
{
    // Run the landing sequence
    // May require a controller to regulate descent etc
}
void ack_cb(comms_payload_t payload)
{
    // fire INITIALISED event
    return;
}
void ir_detected_cb(comms_payload_t payload)
{
    // fire BEAM_DETECTED event
    return;
}
void ir_lost_cb(comms_payload_t payload)
{
    // fire BEAM_LOST event
    return;
}
void landed_cb(comms_payload_t payload)
{
    // fire LANDED event
}
#endif