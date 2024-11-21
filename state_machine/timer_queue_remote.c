#include "timer_queue_remote.h"
#include <math.h>
#include "sub_clock.h"

/* List of timers to service */
timer_node_t* timers_list = NULL;

// Delete the timer from list
int delete_timer(timer_node_t* prev_node, timer_node_t* curr_node) {
    if (prev_node == NULL){
        // No previous node, then set head to null
        timers_list = NULL; // Empty list
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
void add_timer(double duration_ms, event_t trigger_event) {
    // Allocate memory for timer item
    timer_node_t* new_node = malloc(sizeof(timer_node_t));
    new_node->creation_time = getSubMS(); // Get current time
    new_node->next = NULL; // New node at tail so next is NULL
    new_node->duration_ms = duration_ms; // Set duration_ms value
    new_node->trigger_event = trigger_event; // Set event for timer node

    // Add new node to end of list
    if (timers_list == NULL) {
        // If first item in list, list is empty right now
        timers_list = new_node;
    } else {
        timer_node_t* node = timers_list;
        while (node->next != NULL) {
            // Find the last node in list
            node = node->next;
        }
        // Now node is the last node in list
        node->next = new_node;
    }
}

// Iterate through timers list and check for duration_mss
void timer_handler_remote(void) {
    double curr_time;
    timer_node_t* prev_node;
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
            // Delete the timer
            delete_timer(prev_node, node);
        }

        prev_node = node; // Curr node becomes previous
        node = node->next; // Go to next node
    }
}
