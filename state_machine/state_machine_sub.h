/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_SUB_H_
#define __STATE_MACHINE_SUB_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../main.h"

/* MACROS for everyone--------------------------------------------------------*/

typedef enum  {
    DRIVE_MSG_FB_RECEIVED,     // forward/backwards drive command
    DRIVE_MSG_LR_RECEIVED,      // left/right drive command
    DRIVE_MSG_DS_RECEIVED,      // dive/surface drive command
    LAND_MSG_RECEIVED,          // land msg
    RESET_MSG_RECEIVED,        // reset message
    IR_REQUEST_RECEIVED,       // IR status request msg
    SENSOR_POLLING_TIMEOUT     // Timeout for polling sensors
} event_type_t;

typedef struct {
    uint8_t type;
    uint16_t value;
} sub_events_t;

typedef enum {
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
