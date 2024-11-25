/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_QUEUE_REMOTE_H_
#define __TIMER_QUEUE_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "state_machine_remote.h"

/* Types ---------------------------------------------------------*/
// Timer node
typedef struct TimerNode {
    double creation_time;
    double duration_ms;
    remote_event_t trigger_event;
    struct TimerNode* next; // Next node
} timer_node_t; // TimerNode type


/* MACROS for everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/
void timer_handler_remote(void);
int add_timer(double duration_ms, remote_event_t trigger_event);
int delete_timer_by_id(remote_event_t event_to_delete);

#ifdef __cplusplus
}
#endif

#endif /*__TIMER_QUEUE_REMOTE_H */