#pragma once

#include "../main.h"
#include "state_machine_remote.h"
#include <cstdint>

typedef struct queue_element {
    uint8_t event; // Event that fired, either sub or remote
    struct queue_element *next; // Pointer to next element in queue
    struct queue_element *prev; // Pointer to previous element in queue
} queue_element_t;

typedef struct queue{
    queue_element_t *head; // Pointer to first element in queue
    queue_element_t *tail; // Pointer to last element in queue
} queue_t;

typedef enum 
{
    SUB_DEVICE,
    REMOTE_DEVICE
} device_enum;

#define MAX_ELEMENTS 20
typedef struct {
    uint8_t events[MAX_ELEMENTS];
    uint8_t size;
    uint8_t insert_indx;
    uint8_t process_indx;
    device_enum device;
} simple_queue_t;

void init_queue(queue_t *queue);
void pop(queue_t *queue, queue_element_t *element);
void push(queue_t *queue, queue_element_t *element);
void remove_from_queue(queue_t *queue, queue_element_t *element);
void clear(queue_t *queue);

void init_simple_queue(simple_queue_t* simpleQ, device_enum device);
void insert_to_simple_queue(simple_queue_t* simpleQ, uint8_t event);
// void init_simple_queue(remote_events_t event);
void insert_queue(void);
