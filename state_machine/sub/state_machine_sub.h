/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_SUB_H_
#define __STATE_MACHINE_SUB_H_

#include <cstdint>
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* MACROS for everyone--------------------------------------------------------*/

typedef enum  {
    RESET_MSG_RECEIVED,        // reset message
    DRIVE_MSG_FB_RECEIVED,     // forward/backwards drive command
    DRIVE_MSG_LR_RECEIVED,      // left/right drive command
    DRIVE_MSG_DS_RECEIVED,      // dive/surface drive command
    LAND_MSG_RECEIVED,          // land msg
    STATUS_REQUEST_RECEIVED,       // IR status request msg
    SENSOR_POLLING_TIMEOUT     // Timeout for polling sensors
} sub_event_type_t;

typedef struct {
    sub_event_type_t type;
    uint8_t data;
} sub_events_t;

typedef enum {
    IDLE,
    WELCOME,
    DRIVE,
    LANDING,
    RESETTING
} sub_states_t;

typedef struct {
    sub_states_t state;
    uint8_t initialised;
    uint8_t beam_status;
    float current_depth;
    uint8_t land_status;
    uint8_t reset_status;
    uint8_t lr_command;
    uint8_t ds_command;
    uint8_t fb_command;
} sub_t;


#define SENSOR_POLL_TIMEOUT 100

/* Global Variables */
extern sub_t subState;

/*Function definitions---------------------------------------------------------*/
void init_sub(void);
void init_timer(void);
void init_gpio(void);
void event_handler_sub(void);

// debug init funciton 
void init_sub_debugging(sub_states_t testState, uint8_t testBeam);
void poll_sensors_timeout(void);



#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_SUB_H */
