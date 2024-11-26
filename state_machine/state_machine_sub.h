/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_SUB_H_
#define __STATE_MACHINE_SUB_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../main.h"

/* MACROS for everyone--------------------------------------------------------*/

typedef enum  { INITIALISED, // Transition to IDLE
PACKET_RECEIVED, // Behaviour is state dependent
BEAM_DETECTED, // No transition, send beam detected message
BEAM_LOST,  // No transition, send beam lost message
LANDED      // Transition to WELOCME, send landed message
} event_type_t;

typedef struct {
    uint8_t type;
    uint16_t value;
} sub_events_t;

typedef enum {
    INITIALISING,
    IDLE,
    WELCOME,
    DRIVE,
    // BEAM_DRIVE,
    LANDING
} sub_states_t;

typedef struct {
    sub_states_t state;
    uint8_t initialised;
    uint8_t beam_detected;
} sub_t;


/* Global Variables */
extern sub_t subState;

/*Function definitions---------------------------------------------------------*/
void init_sub(void);
void init_timer(void);
void init_gpio(void);
void event_handler_sub();



#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_SUB_H */
