/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_SUB_H_
#define __STATE_MACHINE_SUB_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../main.h"

/* MACROS for everyone--------------------------------------------------------*/


/* Type definisionts and enumerations*/

typedef enum {
    INITIALISED, // Transition to IDLE
    PACKET_RECEIVED, // Behaviour is state dependent
    BEAM_DETECTED, // No transition, send beam detected message
    BEAM_LOST,  // No transition, send beam lost message
    LANDED      // Transition to WELOCME, send landed message
} sub_events_t;

typedef enum {
    INITIALISING,
    IDLE,
    WELCOME,
    DRIVE,
    LANDING
} sub_states_t;

typedef struct {
    sub_states_t state;
    bool initialised;
    bool beam_detected;
} sub_t;


/* Global Variables */
static sub_t subState;

/*Function definitions---------------------------------------------------------*/
void init_sub(void);
void init_timer();
void init_gpio();
void event_handler_sub(void);



#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_SUB_H */