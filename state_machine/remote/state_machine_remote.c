/**
  ******************************************************************************
  * @file    state_machine_sub.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for Hasselhoff Remote
  ******************************************************************************
  */


#include "state_machine_remote.h"
#include "remote_state_callbacks.h"
#include "led_remote.h"
#include "inputs_remote.h"
#include "state_machine_sub.h"
#include "sub_clock.h"
#include "hardware_stm_adc.h"
#include "state_machine_SPI.h"
#include "packet.h"
#include "remote_queue.h"



/* Global Variables ---------------------------------------------------------*/
// Initialize queue
remote_event_t remote_state = INIT_REMOTE; // Global variable of remote's current state
uint8_t sub_status = 0; // Sub's status
/* End Global Variables ---------------------------------------------------------*/

/* Event Scheduler ------------------------------------------------------------------*/
void event_handler_remote(void) {
    /* Checks and handles events for remote */
    // Pop the first event on the queue
    remote_event_t event;
    if ((event = dequeue_by_schedule()) != EMPTY_REMOTE) {
        // READY if queue is empty, else keep popping
        //printf("SUb Status : %d\n",sub_status);
        //printf("Handling EVENT: %d\n", event);
        switch (event) {
            case INIT_REMOTE:
                remote_init_callback();
                break;

            case WELCOME_REMOTE:
                remote_welcome_callback();          
                break;
            
            case DRIVE_REMOTE:
                remote_drive_callback();
                break;
            
            case LAND_REMOTE: // TODO: enqueue the land remote event again
                remote_land_callback();
                break;

            case RESET_REMOTE:
                remote_reset_callback();
            
            case CYCLE_LED_DISPLAY:
                remote_led_display_callback();
                break;
            
            case START_ADC:
                remote_start_adc_callback();
                break;

            case COUNTDOWN_TIMER:
                remote_countdown_timer();
                break;

            case READ_UX:
                remote_read_UX_callback();
                break;
            
            case BEAM_STATUS:
                remote_beam_status_callback();
                break;

            default:
                // Fallback if a case that is not defined
                printf("[ERROR] NOT A VALID EVENT RECEIVED: %d!\n", event);
                break;
        }
    }
}

/* End Event Scheduler ------------------------------------------------------------------*/
void init_remote(void) 
{
    enqueue_event(INIT_REMOTE, 0);
}
