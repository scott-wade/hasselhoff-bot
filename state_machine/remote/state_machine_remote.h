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

/* Types ---------------------------------------------------------*/


/* Global variables --------------------------------------------------------*/
extern remote_event_t remote_state;
extern uint8_t sub_status;

/*Function definitions---------------------------------------------------------*/
void init_remote(void);
void event_handler_remote(void);

#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_REMOTE_H */
