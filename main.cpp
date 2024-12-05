#define WHICH_NUCLEO 0 //change for compilation, 0 for remote, 1 for submarine, 2 for debug remote, 3 for debug sub3, 94 for NZ debugging


#include <cstdint>
#include "main.h"
#include "debug_mort.h"
#include "state_machine/state_machine_sub.h"
#include "state_machine/state_machine_remote.h"
#include "tests/test_spi.h"
#include "tests/test_gpio.h"
#include "state_machine_SPI.h"
#include "inputs_remote.h"
#include "hardware_stm_adc.h"
#include "applications/sub_clock.h"
#include "led_remote.h"
#include "ir_range.h"
#include "timer_queue_remote.h"
#include "motor_controller.h"


int main(void){
    if (WHICH_NUCLEO == 0){
        /* remote state machine */
        sched_event(INIT_REMOTE); // Start with initialization event
        /* loop */
        while(1){
            // Check for any expired timers and update queue
            timer_handler_remote();
            // Check for tasks in queue and then execute them
            event_handler_remote();
            // SPI event handler
            event_handler_spi(NUCLEO_PARENT); // Remote = parent
        }
    } else if(WHICH_NUCLEO == 1) {
        /* submarine state machine */

        /* initialization */
        init_sub(); // State machine
        // Initialise hardware
        initMotorHardware();
        initIRSensor(0); // Initialise IR sensor in digital mode
        // initPressureSensor();

        /* loop */
        while(1){
            timer_handler_remote(); // Identical functionality but different events in the queue
            event_handler_sub();
        }

    }else if(WHICH_NUCLEO == 2){
        /* DEBUGGING CODE PARENT */
        testNucleoTransmitting(NUCLEO_PARENT);

    }else if(WHICH_NUCLEO == 3){
        /* DEBUGGING CODE CHILD */
        testNucleoReceiving(NUCLEO_CHILD);

    }else if (WHICH_NUCLEO == 94){
        /* initialization */
        // initialize the sub clock
        initSubClock();

        // initalize a queue and timeout array (utility and debugging)
        
        /* loop */
        while(1){
            // service event queue
            // check on the timeouts
        }
    }

}
