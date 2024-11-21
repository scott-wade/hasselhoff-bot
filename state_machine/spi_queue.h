#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "../main.h"
#include <cstdint>


#define SPI_QUEUE_MAX_SIZE 10
typedef struct {
    void* data[SPI_QUEUE_MAX_SIZE];
    int front;
    int rear;
    int size;
    size_t elementSize;
} Queue;

// define functions
int isFull(Queue* q);
int isEmpty(Queue* q);
Queue* createQueue(size_t elementSize);
void enqueue(Queue* q, void* item);
void* dequeue(Queue* q);



#ifdef __cplusplus
}
#endif