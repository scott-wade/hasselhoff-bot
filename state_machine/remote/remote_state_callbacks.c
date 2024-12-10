/**
  ******************************************************************************
  * @file    state_machine_sub.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    December -2024
  * @brief   State Machine for Hasselhoff Remote
  ******************************************************************************
  */

#include "remote_state_callbacks.h"
#include "state_machine_remote.h"
#include "remote_queue.h"
#include "led_remote.h"
#include "inputs_remote.h"
#include "packet.h"
#include "state_machine_SPI.h"
#include "sub_clock.h"
#include "hardware_stm_adc.h"

void remote_init_callback(void)
{
    // Initialize the Remote
    initSubClock();
    init_status_leds();
    init_target_depth_knob();
    init_joysticks();
    init_seg_display();
    init_state_machine_spi(NUCLEO_PARENT); // parent = remote
    printf("Initialized remote\n");

    // Startup Events
    // Schedule one-time tasks
    enqueue_event(START_ADC, START_ADC_DELAY_MS);

    // Schedule periodic tasks
    //For the first instance of these tasks, schedule them after the one-time tasks have been executed
    enqueue_event(CYCLE_LED_DISPLAY, START_ADC_DELAY_MS + 10);
    enqueue_event(WELCOME_REMOTE, START_ADC_DELAY_MS + 10);

    
}

void remote_welcome_callback(void)
{
    remote_state = WELCOME_REMOTE;
    welcome_remote_sequence();

    // Schedule Periodic tasks
    enqueue_event(READ_JOYSTICKS, READ_JOYSTICKS_PERIOD_MS);
    // self schedule
    enqueue_event(WELCOME_REMOTE, WELCOME_PERIOD_MS);
}

void remote_drive_callback(void)
{
    remote_state = DRIVE_REMOTE;

    // NEXT: welcome -> driving
    clear_all_leds();
    set_white_led(); // Set driving status LED

    // Schedule periodic events
    enqueue_event(COUNTDOWN_TIMER, COUNTDOWN_TIMER_PERIOD_MS);
    enqueue_event(READ_TARGET_DEPTH, READ_DEPTH_PERIOD_MS);
}

void remote_land_callback(void)
{
    remote_state = LAND_REMOTE;
    clear_white_led(); // Turn off drive status LED
    set_green_led(); // Set landing status LED

    if (sub_status & (1<<1)) {
        // Landing is Done
        
        // Clear Remote queue
        clear_queue();
        // Schedule a Reset event
        enqueue_event(RESET_REMOTE, getSubMS());

        // Notify Submarine
        requestSpiTransmit_remote(RESET_MSG, 0, &sub_status); // send reset message
    } else {

        // query the sub status
        requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &sub_status);
    }
}

void remote_reset_callback(void)
{
    remote_state = RESET_REMOTE;

    if (sub_status & (1<<2)) {
        // Reset is done
        clear_queue();
        enqueue_event(WELCOME_REMOTE, 0);
    } else {

        requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &sub_status);

        // self Schedule
        enqueue_event(RESET_REMOTE, RESET_PERIOD_MS);
    }
}

void remote_start_adc_callback(void)
{
    // Start ADC after a few clock cycle when ADC has started
    startADCConversion(ADC_1);
    startADCConversion(ADC_2);
}

void remote_read_joysticks_callback(void)
{
    uint16_t joy_inputs[2];
    read_joysticks(joy_inputs);

    // If in welcome state and joysticks are within a range, then go to drive state
    if (remote_state == WELCOME_REMOTE){
        if ((joy_inputs[0] <= JOY_ACTIVE_ZONE) || (joy_inputs[0] >= (MAX_JOY_VAL - JOY_ACTIVE_ZONE)) ||
            (joy_inputs[1] <= JOY_ACTIVE_ZONE) || (joy_inputs[1] >= (MAX_JOY_VAL - JOY_ACTIVE_ZONE))) {
            
            // Go Welcome -> Drive (singleton event)
            enqueue_event(DRIVE_REMOTE, getSubMS() + 10);
        }
    } else if (remote_state == DRIVE_REMOTE) {
        
        // Land Activated
        if (land_button_pressed && (sub_status & (1<<0))) {
            clear_queue();
            enqueue_event(LAND_REMOTE, 0);
        }
        
        // When driving, continuously send joystick values
        requestSpiTransmit_remote(DRIVE_LR_MSG, joy_inputs[0], &sub_status); // left/right
        requestSpiTransmit_remote(DRIVE_FB_MSG, joy_inputs[1], &sub_status); // forward/back

    }

    // Self schedule
    enqueue_event(READ_JOYSTICKS, READ_JOYSTICKS_PERIOD_MS);
}

void remote_read_target_depth_callback(void)
{
    if (remote_state != DRIVE_REMOTE){
            return;
    }
            
    uint8_t target_depth = read_target_depth();
    requestSpiTransmit_remote(DRIVE_DS_MSG, target_depth, &sub_status); // drive/surface (up/down)

    // Self schedule
    enqueue_event(READ_TARGET_DEPTH, READ_DEPTH_PERIOD_MS);
}

void remote_led_display_callback(void)
{
    cycle_led_display(); // Cycle thru the 4 digits
    
    // self schedule
    enqueue_event(CYCLE_LED_DISPLAY, DISPLAY_CYCLE_PERIOD_MS);
}

void remote_countdown_timer(void)
{
    if (remote_state != DRIVE_REMOTE) {
        return;
    }
    
    int count = countdown_timer();

    if (count <= 0) {

        // Clear Remote queue
        clear_queue();
        // Schedule a Reset event
        enqueue_event(RESET_REMOTE, getSubMS());

        // Notify Submarine
        requestSpiTransmit_remote(RESET_MSG, 0, &sub_status); // send reset message
        
    } else {
        
        enqueue_event(COUNTDOWN_TIMER, COUNTDOWN_TIMER_PERIOD_MS);
    }
}
        