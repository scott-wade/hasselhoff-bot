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
#include "../applications/packet.h"

/* Constants */

/* Period of periodically occuring tasks */
#define DISPLAY_CYCLE_PERIOD_MS         1
#define READ_DEPTH_PERIOD_MS            100
#define START_ADC_DELAY_MS              100
#define WELCOME_PERIOD_MS               500
#define READ_JOYSTICKS_PERIOD_MS        100
#define COUNTDOWN_TIMER_PERIOD_MS       1000 // 1 sec
#define READ_SUB_STATUS_DELAY_MS        10
#define POLL_SUB_STATUS_PERIOD_MS       100

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



/* Queue ------------------------------------------------------------------*/
// Push new event to end of queue
int sched_event(remote_event_t event) {
    // printf("remote_state: %d\n", remote_state);

    // Allocate memory for queue node
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if (new_node == NULL) {
        fprintf(stderr, "[Error] Failed to allocate memory for new event\n");
        return -1; // error
    }
    // Set new node attributes
    new_node->event = event; // Set event
    new_node->next = NULL; // At end of queue
    new_node->prev = queue.tail; // Point to last tail node

    if (queue.tail == NULL){
        // List is empty, this is the first node so set tail and head
        queue.tail = new_node;
        queue.head = new_node;
    } else {
        // Not empty so add to end of queue
        // Current: tail -> NULL
        // Changed to: tail -> new_node (new tail) -> NULL
        queue.tail->next = new_node;
        queue.tail = new_node;
    }
    // Increment queue's size
    queue.size++;

    return 0; // success
}
// Pop event from start of queue
remote_event_t pop_queue(void){
    if (queue.head == NULL){
        // Nothing in queue, pop should not have been popped!
        return EMPTY_REMOTE;
    }
    // Get the event from the first node
    queue_node_t* head_node = queue.head;
    remote_event_t ret_event = head_node->event;
    // Change the head
    queue.head = head_node->next;
    // Free memory of head
    free(head_node);
    // Decrement size
    queue.size--;
    if (queue.size == 0){
        // If queue is now empty, make sure both head/tail are NULL
        queue.head = NULL;
        queue.tail = NULL;
    }
    return ret_event;
}
/* End Queue ------------------------------------------------------------------*/

/* Event Scheduler ------------------------------------------------------------------*/
// List of all tasks
void tasks(remote_event_t event){
  switch (event) {
        case INIT_REMOTE:
            init_remote();
            // Schedule one-time tasks
            add_timer(START_ADC_DELAY_MS, START_ADC); // Start ADC
            // Schedule periodic tasks to the queue
            sched_event(CYCLE_LED_DISPLAY);
            add_timer(START_ADC_DELAY_MS + 10, READ_JOYSTICKS); // Start after ADC
            sched_event(POLL_SUB_STATUS);
            // NEXT: Init -> Welcome
            sched_event(WELCOME_REMOTE);
            break;
        case WELCOME_REMOTE:
            remote_state = WELCOME_REMOTE;
            welcome_remote();
            add_timer(WELCOME_PERIOD_MS, WELCOME_REMOTE); // Add event back on queue as a periodic task            
            break;
        case DRIVE_REMOTE:
            remote_state = DRIVE_REMOTE;
            // Delete the periodic welcome state from timer queue to stop it
            delete_timer_by_id(WELCOME_REMOTE);
            // NEXT: welcome -> driving
            clear_all_leds();
            set_white_led(); // Set driving status LED
            sched_event(COUNTDOWN_TIMER); // Start countdown timer
            sched_event(READ_TARGET_DEPTH); // Read depth knob
            break;
        case LAND_REMOTE:
            remote_state = LAND_REMOTE;
            clear_white_led(); // Turn off drive status LED
            set_green_led(); // Set landing status LED
            break;
        case CYCLE_LED_DISPLAY:
            cycle_led_display(); // Cycle thru the 4 digits
            add_timer(DISPLAY_CYCLE_PERIOD_MS, CYCLE_LED_DISPLAY); // Add event back on queue as a periodic task
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
            add_timer(READ_DEPTH_PERIOD_MS, READ_TARGET_DEPTH); // Add event back on queue as a periodic task
            break;
        case COUNTDOWN_TIMER:
            if (remote_state != DRIVE_REMOTE)
                break;
            countdown_timer();
            add_timer(COUNTDOWN_TIMER_PERIOD_MS, COUNTDOWN_TIMER); // Add event back on queue as a periodic task
            break;
        case READ_JOYSTICKS:
            read_joysticks();
            add_timer(READ_JOYSTICKS_PERIOD_MS, READ_JOYSTICKS); // Add event back on queue as a periodic task
            break;
        case POLL_SUB_STATUS:
            requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &sub_status);
            add_timer(READ_SUB_STATUS_DELAY_MS, READ_SUB_STATUS); // Read status after some delay
            break;
        case READ_SUB_STATUS:
            read_sub_status();
            add_timer(POLL_SUB_STATUS_PERIOD_MS, POLL_SUB_STATUS); // Poll sub status again
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
    while ((event = pop_queue()) != EMPTY_REMOTE) {
        // READY if queue is empty, else keep popping
        tasks(event);
    }
}

/* End Event Scheduler ------------------------------------------------------------------*/
