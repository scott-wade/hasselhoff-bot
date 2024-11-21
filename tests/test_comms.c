#include "test_comms.h"
#include "comms.h"
#include "queue.h"
#include "state_machine_sub.h"
#include <cstdint>


void test_comms(void)
{
    printf("==========================\n");
    init_sub();

    init_simple_queue(SUB_DEVICE);
    // // State machine test sequence
    sub_events_t event;
    comms_payload_t packet;
    // 1. INITIALISED
    event = INITIALISED;
    insert_to_simple_queue(event);
    // 2. PACKET_RECEIVED
    packet.type = ACK;
    // debug_packet = (uint8_t*) &packet;
    set_debug_packet(&packet);
    event = PACKET_RECEIVED;
    insert_to_simple_queue(event);
    // 3. BEAM_DETECTED
    event = BEAM_DETECTED;
    insert_to_simple_queue(event);
    // 4. BEAM_LOST
    event = BEAM_LOST;
    insert_to_simple_queue(event);
    // 5. PACKET_RECEIVED
    packet.type = LAND_CMD;
    set_debug_packet(&packet);
    event = PACKET_RECEIVED;
    insert_to_simple_queue(event);
    // 6. BEAM_DETECTED
    event = BEAM_DETECTED;
    insert_to_simple_queue(event);
    // 7. PACKET_RECEIVED
    packet.type = LAND_CMD;
    set_debug_packet(&packet);
    event = PACKET_RECEIVED; 
    insert_to_simple_queue(event);
    
    printf("Running event handler...\n");
    printf("___________________________\n");
    for(int i = 0; i < 7; i++)
    {
        event_handler_sub();
        delay(20);
    }
    printf("Done");
    
}