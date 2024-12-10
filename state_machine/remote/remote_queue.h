
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REMOTE_QUEUE_H_
#define __REMOTE_QUEUE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

// Queue node
typedef struct {
    double creation_time;
    double duration_ms;
    remote_event_t event;
    struct QueueNode* next; // Next node
    struct QueueNode* prev; // Previous node
} queue_node_t; // QueueNode type

// Defintion of FIFO queue
struct queue_remote_t {
    queue_node_t* head;
    queue_node_t* tail;
    uint32_t size;
};

/* Functions  ------------------------------------------------------------------*/
void enqueue(remote_event_t event);
remote_event_t dequeue_by_schedule(void);
void clear_queue(void);


#ifdef __cplusplus
}
#endif

#endif /*__TIMER_QUEUE_H */