#define WHICH_NUCLEO 94 //change for compilation, 0 for remote, 1 for submarine, 2 for debug remote, 3 for debug sub3, 94 for NZ debugging


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

        init_remote();
        
        /* loop */
        while(1){
            // Check for tasks in queue and then execute them
            event_handler_remote();
            
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
        //initButtonIntInput();    
        // queue up initial pressure readings
        //initPressure();
        /* loop */
        while(1){
            // service event queue
            
            for(int i=0; i<100000; i++){
                event_handler_spi(SENSOR_PARENT); // handles the SPI transmissions
                //printf("Port %u Pin %u \n", CS_PINS[0], CS_PINS[1]);
                //printf("pin G2 output %u \n", readGPIOoutput(CS_PINS[0], CS_PINS[1]));
            }
            //validateSensorInitMsg(); // print out results of who am i
            //printf("pressure %.2f\n", getPressure());
            //extraWhoAmICheck(); // queue up another who am i
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

    else if (WHICH_NUCLEO == 5) {
        init_sub();
        while(1) {
            planarControl(0.5, 0.2);
            depthControl(0.5);
        }
    }

}
