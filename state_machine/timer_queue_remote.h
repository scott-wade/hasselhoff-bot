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
    double duration;
    event_t trigger_event;
    struct TimerNode* next; // Next node
} timer_node_t; // TimerNode type

/* MACROS for everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/
void timer_handler_remote(void);


#ifdef __cplusplus
}
#endif

#endif /*__TIMER_QUEUE_REMOTE_H */
