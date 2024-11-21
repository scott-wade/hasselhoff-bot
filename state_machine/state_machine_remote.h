/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_REMOTE_H_
#define __STATE_MACHINE_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Types ---------------------------------------------------------*/
// List of all events in state machine
typedef enum {
    READY
} event_t;

// Queue node
struct QueueNode {
    event_t event;
    struct QueueNode* next; // Next node
    struct QueueNode* prev; // Previous node
};
typedef struct QueueNode queue_node_t; // QueueNode type

// Defintion of FIFO queue
struct queue_remote_t {
    queue_node_t* head;
    queue_node_t* tail;
    uint32_t size;
};

/* MACROS for everyone--------------------------------------------------------*/



/*Function definitions---------------------------------------------------------*/
void init_remote(void);
void event_handler_remote(void);
void sched_event(event_t event);

#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_REMOTE_H */
