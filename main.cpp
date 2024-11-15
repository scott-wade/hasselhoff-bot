#define WHICH_NUCLEO 2 //change for compilation, 0 for remote, 1 for submarine, 2 for debug

#include "main.h"
#include "debug_mort.h"
#include "state_machine/state_machine_sub.h"
#include "state_machine/state_machine_remote.h"
#include "tests/test_spi.h"
#include "tests/test_gpio.h"


int main(void){

    if (WHICH_NUCLEO == 0){
        /* remote state machine */

        /* initialization */
        init_remote();

        /* loop */
        while(1){
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
        /* DEBUGGING CODE */
        //testB0Set();
        //testB0Clear();
        testReadWriteRegOpMode();
    }

}
