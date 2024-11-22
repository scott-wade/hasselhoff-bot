#include "timer_queue_remote.h"
#include <math.h>
#include "sub_clock.h"

/* List of timers to service */
timer_node_t* timers_list = NULL;

// Delete the timer from list
int delete_timer(timer_node_t* prev_node, timer_node_t* curr_node) {
    if (prev_node == NULL){
        // No previous node, then set head to null
        timers_list = curr_node->next; // Empty list
    } else {
        // Change prev -> curr -> next
        // Skip curr: prev -> next
        prev_node->next = curr_node->next;
    }
    // Free curr node memory
    free(curr_node);
    return 0; // Success
}

// Add new timer to list
int add_timer(double duration_ms, remote_event_t trigger_event) {
    // Allocate memory for timer item
    timer_node_t* new_node = malloc(sizeof(timer_node_t));
    if (new_node == NULL) {
        fprintf(stderr, "[Error] Failed to allocate memory for new timer\n");
        return -1; // error
    }
    // Set new node attributes
    new_node->creation_time = getSubMS(); // Get current time
    new_node->duration_ms = duration_ms; // Set duration value
    new_node->trigger_event = trigger_event; // Set event for timer node

    // Add new node to start of list
    new_node->next = timers_list; // next is old head
    timers_list = new_node; // new head is the new node

    return 0; // success
}

// Iterate through timers list and check for duration_mss
void timer_handler_remote(void) {
    double curr_time;
    timer_node_t* prev_node = NULL;
    // Start at list head and iterate through all active timers
    timer_node_t* node = timers_list;
    while (node != NULL) {
        // Get current time
        curr_time = getSubMS();

        // Take floating abs val of current time - creation time
        if (fabs(curr_time - node->creation_time) > node->duration_ms){
            // If difference > duration_ms, then timer has timed out!
            // Schedule the trigger event
            sched_event(node->trigger_event);   
            // Move to next node before deleting current one
            timer_node_t* to_delete = node;
            node = node->next;
            // Delete the timer node
            delete_timer(prev_node, to_delete);
            continue; // next iteration
        }

        prev_node = node; // Curr node becomes previous
        node = node->next; // Go to next node
    }
}
