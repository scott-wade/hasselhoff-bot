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
#include "timer_queue_remote.h"


int main(void){

    if (WHICH_NUCLEO == 0){
        /* remote state machine */

        /* initialization */
        init_remote();
        
        // int display_vals[4] = {1, 2, 3, 4};
        // set_led_display(display_vals);
        
        /* loop */
        while(1){
            startADCConversion(ADC_1);
            startADCConversion(ADC_2);

            printf("TARGET DEPTH: %u\n", get_target_depth());
            printf("JOYSTICK: [%u, %u]\n", get_joystick_x(), get_joystick_y());

            // cycle_led_display();
            delay(10);

            // Check for any expired timers and update queue
            timer_handler_remote();
            // Check for tasks in queue and then execute them
            event_handler_remote();
            
        }

    }else if(WHICH_NUCLEO == 1) {
        /* submarine state machine */

        /* initialization */
        init_sub();

        /* loop */
        while(1){
            event_handler_sub();
        }

    }else if(WHICH_NUCLEO == 2){
        /* DEBUGGING CODE PARENT */
        //testB0Set();
        //testB0Clear();
        //testReadWriteRegOpMode();
        //testSPIStateMachine();
        testNucleoTransmitting();

    }else if(WHICH_NUCLEO == 3){
        /* DEBUGGING CODE CHILD */
        //testB0Set();
        //testB0Clear();
        //testReadWriteRegOpMode();
        //testSPIStateMachine();
        testNucleoReceiving();

    }else if (WHICH_NUCLEO == 94){
        /* initialization */
        // initialize the sub clock
        initSubClock();
        // initialize my gpio for debugging
        // initalize a queue and timeout array (utility and debugging)
        
        /* loop */
        while(1){
            // service event queue
            // check on the timeouts
        }
    }


}
