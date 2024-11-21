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
#include "sub_clock.h"
#include "timer_queue_remote.h"

/* Constants */
#define DISPLAY_CYCLE_PERIOD_MS     1

/* Global Variables ---------------------------------------------------------*/
// Initialize queue
struct queue_remote_t queue = {
    .head = NULL,
    .tail = NULL,
    .size = 0
};
/* End Global Variables ---------------------------------------------------------*/


void init_remote(void){
    /* Initialize remote state machine */ 
    initSubClock();

    init_status_leds();
    init_target_depth_knob();
    init_joysticks();
    init_seg_display();
}



/* Queue ------------------------------------------------------------------*/
// Push new event to end of queue
void sched_event(event_t event) {
    // Allocate memory for queue node
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
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
}
// Pop event from start of queue
event_t pop_queue(void){
    if (queue.head == NULL){
        // Nothing in queue, pop should not have been popped!
        return READY;
    }
    // Get the event from the first node
    queue_node_t* head_node = queue.head;
    event_t ret_event = head_node->event;
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
void tasks(event_t event){
  switch (event) {
        case INIT:
            init_remote();
            sched_event(CYCLE_LED_DISPLAY); // Cycle the display
            break;
        case CYCLE_LED_DISPLAY:
            cycle_led_display();
            // Add event back on queue
            add_timer(DISPLAY_CYCLE_PERIOD_MS, CYCLE_LED_DISPLAY);
            break;
        default:
            // Fallback if a case that is not defined
            printf("[ERROR] NOT A VALID EVENT RECEIVED: %d!\n", event);
            break;
    }
}
void event_handler_remote(void){
    /* Checks and handles events for remote */
    // Pop the first event on the queue
    event_t event;
    while ((event = pop_queue()) != READY) {
        // READY if queue is empty, else keep popping
        tasks(event);
    }
}
void welcome_state (void)
{
    // Light up LED status lights
    int led_i = 0;
    switch(led_i) {
        case 0:
            clear_rgb_green_led();
            clear_rgb_red_led();
            set_white_led();
            break;
        case 1:
            clear_white_led();
            set_blue_led();
            break;
        case 2:
            clear_blue_led();
            set_yellow_led();
            break;
        case 3:
            clear_yellow_led();
            set_green_led();
            break;
        case 4:
            clear_green_led();
            set_rgb_green_led();
            set_rgb_red_led();
            break;
    }
    led_i = (led_i+1)%5;

}
/* End Event Scheduler ------------------------------------------------------------------*/
