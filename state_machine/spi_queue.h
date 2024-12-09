#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "../main.h"
#include <cstdint>

typedef struct{
    uint8_t child_id; //SPI ID
    uint16_t tx_packet;
    uint8_t* read_var_addr;
} transmitEvent;


#define SPI_QUEUE_MAX_SIZE 10
typedef struct {
    transmitEvent data[SPI_QUEUE_MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;

// define functions
int isFull(Queue q);
int isEmpty(Queue q);
Queue createQueue();
void enqueue(Queue q, transmitEvent item);
transmitEvent dequeue(Queue q);




#ifdef __cplusplus
}
#endif
