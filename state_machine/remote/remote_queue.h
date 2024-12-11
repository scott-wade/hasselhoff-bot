
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REMOTE_QUEUE_H_
#define __REMOTE_QUEUE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum {
    // Main events
    EMPTY_REMOTE,
    INIT_REMOTE,
    WELCOME_REMOTE,
    DRIVE_REMOTE,
    LAND_REMOTE,
    RESET_REMOTE,

    // Other events
    START_ADC,
    CYCLE_LED_DISPLAY,
    COUNTDOWN_TIMER,
    READ_UX,
    BEAM_STATUS
} remote_event_t;

// Queue node
typedef struct queue_node_t {
    double creation_time;
    double duration_ms;
    remote_event_t event;
    struct queue_node_t* next; // Next node
    struct queue_node_t* prev; // Previous node
} queue_node_t; // QueueNode type

// Defintion of FIFO queue
typedef struct queue_remote_t {
    queue_node_t* head;
    queue_node_t* tail;
    uint32_t size;
} queue_remote_t;

extern queue_remote_t queue;

/* Functions  ------------------------------------------------------------------*/
void enqueue_event(remote_event_t event, double duration_ms);
remote_event_t dequeue_by_schedule(void);
void clear_queue(void);


#ifdef __cplusplus
}
#endif

#endif /*__TIMER_QUEUE_H */