/**
  ******************************************************************************
  * @file    spi_queue.c 
  * @author  stwade@andrew.cmu.edu
  * @version 1.0
  * @date    October-2024
  * @brief   Queue implementation for the SPI state machine
  ******************************************************************************
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_queue.h"


int isFull(Queue* q) {
    return q->size == SPI_QUEUE_MAX_SIZE;
}

int isEmpty(Queue* q) {
    return q->size == 0;
}

Queue createQueue() {
    Queue q;
    q.front = 0;
    q.rear = -1;
    q.size = 0;
    return q;
}

void enqueue(Queue* q, transmitEvent item) {
    if (isFull(q)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    
    q->rear = (q->rear + 1) % SPI_QUEUE_MAX_SIZE;
    q->data[q->rear] = item;
    q->size++;
}

transmitEvent dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty. Cannot dequeue.\n");
    }
    
    transmitEvent item = q->data[q->front];
    q->front = (q->front + 1) % SPI_QUEUE_MAX_SIZE;
    q->size--;
    return item;
}
