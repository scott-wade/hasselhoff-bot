/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_REMOTE_H_
#define __STATE_MACHINE_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "remote_queue.h"

/* Constants */

/* Period of periodically occuring tasks */
#define DISPLAY_CYCLE_PERIOD_MS         1
#define READ_DEPTH_PERIOD_MS            100
#define START_ADC_DELAY_MS              100
#define WELCOME_PERIOD_MS               500
#define READ_JOYSTICKS_PERIOD_MS        100
#define COUNTDOWN_TIMER_PERIOD_MS       1000 // 1 sec
#define READ_SUB_STATUS_DELAY_MS        10
#define POLL_SUB_STATUS_PERIOD_MS       100

/* Types ---------------------------------------------------------*/
// List of all events in state machine


/* Global variables --------------------------------------------------------*/
extern remote_event_t remote_state;
extern uint8_t sub_status;

/*Function definitions---------------------------------------------------------*/
void init_remote(void);
void event_handler_remote(void);
int sched_event(remote_event_t event);

#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_REMOTE_H */
