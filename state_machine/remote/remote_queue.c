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

    if (queue.head == NULL) {
        // First Node 
        queue.head = new_node;
        queue.tail = new_node;
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
    int k = 0;

    while (curr_node != NULL) {

        // Parse attributes of current node
        duration_ms = curr_node -> duration_ms;
        creation_time = curr_node->creation_time;

        printf("Iteration: %d, Current Time %f, Creation Time %f, Duration %f, event : %d\n", k, current_time, creation_time, duration_ms, curr_node->event);

        if (fabs(current_time - creation_time) >= duration_ms) {

            ret_event = curr_node -> event;

            if (curr_node -> prev != NULL) {
                curr_node->prev->next = curr_node->next;
            }

            if (curr_node -> next != NULL) {
                curr_node->next->prev = curr_node->prev;
            }

            if (queue.head == curr_node) {
                queue.head = curr_node -> next;
            }

            free(curr_node);
            queue.size--;

            break;
        }
        // Not the event we want to pop, continue to the next one
        curr_node = curr_node->next;
        k++;
    }
    return ret_event;
}

void clear_queue(void)
{
    queue_node_t* curr_node = queue.head;
    queue_node_t* temp;

    while(curr_node != NULL) {
      temp = curr_node -> next;
      free(curr_node);
      curr_node = temp;
    }

    queue.head = NULL;
    queue.tail = NULL;
    queue.size = 0;
}