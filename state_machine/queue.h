#pragma once

#include "../main.h"
#include "state_machine_remote.h"
#include "state_machine_sub.h"
#include <cstdint>

// typedef struct queue_element {
//     uint8_t event; // Event that fired, either sub or remote
//     struct queue_element *next; // Pointer to next element in queue
//     struct queue_element *prev; // Pointer to previous element in queue
// } queue_element_t;

// typedef struct queue{
//     queue_element_t *head; // Pointer to first element in queue
//     queue_element_t *tail; // Pointer to last element in queue
// } queue_t;

typedef enum 
{
    SUB_DEVICE,
    REMOTE_DEVICE
} device_enum;

#define MAX_ELEMENTS 20
typedef struct {
    sub_events_t events[MAX_ELEMENTS];
    uint8_t size;
    uint8_t insert_indx;
    uint8_t remove_indx;
    device_enum device;
} simple_queue_t;

void init_simple_queue(device_enum device);
void insert_to_simple_queue(sub_events_t event);
sub_events_t pop_from_simple_queue(void);

uint8_t isempty_simple_queue(void);
uint8_t isfull_simple_queue(void);
void clear_simple_queue(void);
