#define WHICH_NUCLEO 0 //change for compilation, 0 for remote, 1 for submarine

#include "main.h"
#include "debug_mort.h"
#include "state_machine/state_machine_sub.h"
#include "state_machine/state_machine_remote.h"


int main(void){

    if (WHICH_NUCLEO == 0){
        /* remote state machine */

        /* initialization */
        init_remote();

        /* loop */
        while(1){
            event_handler_remote();
        }

    }else {
        /* submarine state machine */

        /* initialization */
        init_sub();

        /* loop */
        while(1){
            event_handler_sub();
        }

    }    

}