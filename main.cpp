#define WHICH_NUCLEO 0 //change for compilation, 0 for remote, 1 for submarine, 2 for debug remote, 3 for debug sub3, 94 for NZ debugging


#include <cstdint>
#include "main.h"
#include "state_machine_sub.h"
#include "state_machine_remote.h"

#include "state_machine_SPI.h"
#include "inputs_remote.h"
#include "hardware_stm_adc.h"
#include "sub_clock.h"
#include "depth_sensor.h"
#include "led_remote.h"
#include "ir_range.h"
#include "motor_controller.h"
#include <cmath>
#include <iostream>

#include "test_spi.h"
#include "test_gpio.h"
#include "test_depth.h"
#include "test_sub_state_machine.h"

int main(void){
    if (WHICH_NUCLEO == 0){

        printf("Starting Remote\n");
        init_remote();

        printf("Initialized Remote\n");
        
        /* loop */
        while(1){
            // Check for tasks in queue and then execute them
            event_handler_remote();
            printf("Handling Event\n");
            
            // SPI event handler
            event_handler_spi(NUCLEO_PARENT); // Remote = parent
        }
    } 
    
    
    else if(WHICH_NUCLEO == 1) {
        /* submarine state machine */

        // initialization
        init_sub(); // State machine
        
        // loop
        while(1){
            event_handler_sub();
        }
    }






// *********************************************************************************************************************
// DEBUGGING

    else if(WHICH_NUCLEO == 2){
        /* DEBUGGING CODE PARENT */
        testNucleoTransmitting(NUCLEO_PARENT);

    }else if(WHICH_NUCLEO == 3){
        /* DEBUGGING CODE CHILD */
        testNucleoReceiving(NUCLEO_CHILD);

    }else if (WHICH_NUCLEO == 94){
        /* initialization */
        // initialize the sub clock
        initSubClock();    
        // initialize SPI comm to sensor(s)
        init_state_machine_spi(SENSOR_PARENT); // this is a home for the spi queue    
        // initalize a sub state machine queue and timeout array
        // initialize the depth sensor settings
        initPressureSensorSettings();
        // initialize my gpio for debugging
        initButtonIntInput();    
        // queue up initial pressure readings
        initPressure();
        /* loop */
        while(1){
            // service event queue
            event_handler_spi(SENSOR_PARENT); // handles the SPI transmissions
            // check on the timeouts
        }
    } else if (WHICH_NUCLEO == 4)
    {
        printf("Initialising......\n");
        init_sub();
        printf("Initialised SUB ......\n");
        init_state_machine_spi(SENSOR_PARENT);
        printf("Initialised SENSOR_PARENT ......\n");
        init_state_machine_spi(NUCLEO_CHILD);
        printf("Initialised NUCLEO CHILD......\n");

        while(1)
        {
            event_handler_spi(SENSOR_PARENT);
            event_handler_spi(NUCLEO_CHILD);
            event_handler_sub();
        }
    }

}
