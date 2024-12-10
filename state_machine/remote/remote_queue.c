/**
  ******************************************************************************
  * @file    timer_queue.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    December -2024
  * @brief   State Machine for Hasselhoff Remote
  ******************************************************************************
  */

#include "remote_queue.h"
#include "sub_clock.h"
#include "math.h"

// Gloabl Remote Queue
struct queue_remote_t queue = {
    .head = NULL,
    .tail = NULL,
    .size = 0
};

// Push new event to end of queue
void enqueue_event(remote_event_t event, double duration_ms) 
{
    // Allocate memory for queue node
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if (new_node == NULL) {
        fprintf(stderr, "[Error] Failed to allocate memory for new event\n");
    }
    // Set new node attributes
    new_node->event = event; // Set event
    new_node->duration_ms = duration_ms;
    new_node->creation_time = getSubMS();
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

remote_event_t dequeue_by_schedule(void)
{
    remote_event_t ret_event = EMPTY_REMOTE;
    double duration_ms;
    double creation_time;
    queue_node_t* curr_node = queue.head;

    double current_time = getSubMS();

    while (curr_node != NULL) {

        // Parse attributes of current node
        duration_ms = curr_node -> duration_ms;
        creation_time = curr_node->creation_time;

        if (fabs(current_time - creation_time) >= duration_ms) {

            ret_event = curr_node -> event;

            if (curr_node -> prev != NULL) {
                curr_node->prev->next = curr_node->next;
            } else {
                
            }

            if (curr_node -> next != NULL) {
                curr_node->next->prev = curr_node->prev;
            }

            if (queue.head == curr_node) {
                queue.head = curr_node -> next;
            }

            free(curr_node);
            queue.size--;

            if (queue.size == 0) {
                queue.head = NULL;
                queue.tail = NULL;
            }

            break;
        } else {
            // Not the event we want to pop, continue to the next one
            curr_node = curr_node->next;
        }

    }
    return ret_event;
}

void clear_queue(void)
{
    queue_node_t* curr_node = queue.head;

    while(curr_node != NULL) {
      queue_node_t* temp = curr_node;
      curr_node = curr_node -> next;
      free(temp);
      queue.size --;
    }
}