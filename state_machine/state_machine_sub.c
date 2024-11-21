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
#include "comms.h"
#include "globals.h"

void init_sub(void){
    /* Initialize sub state machine */ 
    subState.state = INITIALISING;
    subState.initialised = 0;
    subState.beam_detected = 0;
}


void event_handler_sub(){
    /* Checks and handles events for sub */
    if (simpleQ.size > 0)
    {
        printf("Queue has %d events\n", simpleQ.size);
    } else {
        printf("No events queued\n");
        return;
    }

    
    sub_events_t current_event = simpleQ.events[simpleQ.process_indx];

    printf("Processing event %d\n", simpleQ.process_indx);
    simpleQ.process_indx++;
    simpleQ.size--;
    if (simpleQ.process_indx == MAX_ELEMENTS)
    {
        simpleQ.process_indx = 0;
    }

    printf("Event is: %d\n", current_event.type);

    switch (current_event.type) {
        case INITIALISED:
            printf("INITIALISED EVENT\n");
            subState.initialised = 1;
            subState.state = IDLE;
            break;
        case PACKET_RECEIVED:
            printf("PACKET RECVD\n");
            recv_comms_debug();
            break;
        case BEAM_DETECTED:
            printf("BEAM_DETECT\n");
            subState.beam_detected = 1;
            break;
        case BEAM_LOST:
            printf("BEAM_LOST\n");
            subState.beam_detected = 0;
            break;
        case LANDED:
            break;
        default:
            break;
    }
    printf("--------------------\n");


}