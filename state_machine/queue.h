#pragma once

#include "../main.h"
#include "state_machine_sub.h"
#include "state_machine_remote.h"

typedef struct queue_element{
    union {
        sub_events_t sub_event;
        remote_events_t remote_event;
    } event; // Event that fired, either sub or remote
    queue_element_t *next; // Pointer to next element in queue
    queue_element_t *prev; // Pointer to previous element in queue
} queue_element_t;

typedef struct queue{
    queue_element_t *head; // Pointer to first element in queue
    queue_element_t *tail; // Pointer to last element in queue
} queue_t;

void init_queue(queue_t *queue);
void pop(queue_t *queue, queue_element_t *element);
void push(queue_t *queue, queue_element_t *element);
void remove(queue_t *queue, queue_element_t *element);
void clear(queue_t *queue);

