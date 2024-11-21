/**
  ******************************************************************************
  * @file    state_machine_sub.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for Hasselhoff Remote
  ******************************************************************************
  */


#include "state_machine_remote.h"
#include "led_remote.h"
#include "inputs_remote.h"

void init_remote(void){
    /* Initialize remote state machine */ 
    init_status_leds();
    init_target_depth_knob();
    init_joysticks();
    init_seg_display();
}


void event_handler_remote(void){
    /* Checks and handles events for remote */


}

