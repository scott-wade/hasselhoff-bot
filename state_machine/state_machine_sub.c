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
#include "queue.h"

void init_sub(void){
    /* Initialize sub state machine */ 
    subState.state = INITIALISING;
    subState.initialised = false;
    subState.beam_detected = false;
}


void event_handler_sub(void){
    /* Checks and handles events for sub */
    sub_events_t current_event = simpleQ.events[simpleQ.process_indx].sub_event;
    simpleQ.process_indx++;
    if (simpleQ.process_indx == MAX_ELEMENTS)
    {
        simpleQ.process_indx = 0;
    }

    switch (current_event) {
        case INITIALISED:
            subState.initialised = true;
            subState.state = IDLE;
            break;
        default:
            break;
    }


}