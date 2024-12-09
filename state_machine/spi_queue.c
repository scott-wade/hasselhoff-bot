
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_queue.h"


int isEmpty(Queue* q) {
    return q->front == NULL;
}

Queue* createQueue(size_t elementSize) {
    Queue *q = malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, void* data) {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

void* dequeue(Queue* q) {
    if (q->front == NULL) 
        return NULL;

    Node *temp = q->front;
    void *data = temp->data;
    q->front = q->front->next;

    if (q->front == NULL) 
        q->rear = NULL;

    free(temp);
    return data;
}
