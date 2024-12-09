#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "../main.h"
#include <cstdint>


#define SPI_QUEUE_MAX_SIZE 10

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

// define functions
int isEmpty(Queue* q);
Queue* createQueue(size_t elementSize);
void enqueue(Queue* q, void* item);
void* dequeue(Queue* q);



#ifdef __cplusplus
}
#endif
