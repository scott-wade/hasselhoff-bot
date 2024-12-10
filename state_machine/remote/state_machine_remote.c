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
#include "state_machine_sub.h"
#include "sub_clock.h"
#include "timer_queue_remote.h"
#include "hardware_stm_adc.h"
#include "state_machine_SPI.h"
#include "packet.h"
#include "remote_queue.h"



/* Global Variables ---------------------------------------------------------*/
// Initialize queue
struct queue_remote_t queue = {
    .head = NULL,
    .tail = NULL,
    .size = 0
};
remote_event_t remote_state = INIT_REMOTE; // Global variable of remote's current state
uint8_t sub_status = 0; // Sub's status
/* End Global Variables ---------------------------------------------------------*/


void init_remote(void){
    /* Initialize remote state machine */ 
    // Init resources /////////////////////////
    initSubClock();
    init_status_leds();
    init_target_depth_knob();
    init_joysticks();
    init_seg_display();
    init_state_machine_spi(NUCLEO_PARENT); // parent = remote
    printf("Initialized remote\n");
}

/* Event Scheduler ------------------------------------------------------------------*/
// List of all tasks
void tasks(remote_event_t event){
  switch (event) {
        case INIT_REMOTE:
            init_remote();
            // Schedule one-time tasks
            enqueue(START_ADC, START_ADC_DELAY_MS);

            // Schedule periodic tasks
            //For the first instance of these tasks, schedule them after the one-time tasks have been executed
            enqueue(CYCLE_LED_DISPLAY, START_ADC_DELAY_MS + 10);
            enqueue(READ_JOYSTICKS, START_ADC_DELAY_MS + 10);
            enqueue(POLL_SUB_STATUS, START_ADC_DELAY_MS + 10);
            enqueue(WELCOME_REMOTE, START_ADC_DELAY_MS + 10);
            break;

        case WELCOME_REMOTE:
            remote_state = WELCOME_REMOTE;
            welcome_remote();
            enqueue(WELCOME_REMOTE, WELCOME_PERIOD_MS);           
            break;
        
        case DRIVE_REMOTE:
            remote_state = DRIVE_REMOTE;

            // NEXT: welcome -> driving
            clear_all_leds();
            set_white_led(); // Set driving status LED

            enqueue(COUNTDOWN_TIMER, COUNTDOWN_TIMER_PERIOD_MS);
            enqueue(READ_TARGET_DEPTH, READ_DEPTH_PERIOD_MS);
            break;
        
        case LAND_REMOTE: // TODO: enqueue the land remote event again
            remote_state = LAND_REMOTE;
            clear_white_led(); // Turn off drive status LED
            set_green_led(); // Set landing status LED
            break;
        
        case CYCLE_LED_DISPLAY:
            cycle_led_display(); // Cycle thru the 4 digits
            enqueue(CYCLE_LED_DISPLAY, DISPLAY_CYCLE_PERIOD_MS);
            break;
        
        case START_ADC:
            // Start ADC after a few clock cycle when ADC has started
            startADCConversion(ADC_1);
            startADCConversion(ADC_2);
            break;
        
        case READ_TARGET_DEPTH:
            if (remote_state != DRIVE_REMOTE) 
                break;
            read_target_depth();
            enqueue(READ_TARGET_DEPTH, READ_DEPTH_PERIOD_MS);
            break;

        case COUNTDOWN_TIMER:
            if (remote_state != DRIVE_REMOTE)
                break;
            countdown_timer();
            enqueue(COUNTDOWN_TIMER, COUNTDOWN_TIMER_PERIOD_MS);
            break;

        case READ_JOYSTICKS:
            read_joysticks();
            enqueue(READ_JOYSTICKS, READ_JOYSTICKS_PERIOD_MS);
            break;

        case POLL_SUB_STATUS:
            requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &sub_status);
            enqueue(READ_SUB_STATUS, READ_SUB_STATUS_DELAY_MS);
            break;

        case READ_SUB_STATUS:
            read_sub_status();
            enqueue(POLL_SUB_STATUS, POLL_SUB_STATUS_PERIOD_MS);
            break;

        default:
            // Fallback if a case that is not defined
            printf("[ERROR] NOT A VALID EVENT RECEIVED: %d!\n", event);
            break;
    }
}
void event_handler_remote(void) {
    /* Checks and handles events for remote */
    // Pop the first event on the queue
    remote_event_t event;
    while ((event = dequeue_by_schedule()) != EMPTY_REMOTE) {
        // READY if queue is empty, else keep popping
        tasks(event);
    }
}

/* End Event Scheduler ------------------------------------------------------------------*/
