#include <cstdint>
#define WHICH_NUCLEO 2 //change for compilation, 0 for remote, 1 for submarine, 2 for debug

#include "main.h"
#include "debug_mort.h"
#include "state_machine/state_machine_sub.h"
#include "state_machine/state_machine_remote.h"
#include "tests/test_spi.h"
#include "tests/test_gpio.h"
#include "state_machine_SPI.h"


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
        //testReadWriteRegOpMode();

        // initialize SPI state machine
        init_state_machine_spi();

        // init debugging packet 
        uint8_t wnr = 0; // read
        uint8_t data = (uint8_t)(0b1 << 7);
        uint8_t addr = 0x01;
        uint8_t addr_packed = (uint8_t)(wnr << 7 | addr);
        uint16_t packet = (uint16_t)(addr_packed << 8 | data);
        printf("packet: %u\n", packet);

        // var to read from spi to
        uint16_t spi_rx = 0;

        // run state machine
        int delaything = 0;

        while(1){
            event_handler_spi();
            delaything = delaything + 1;
            if(delaything == 100000){
                requestSpiTransmit(1, packet, &spi_rx);
                delaything=0;
            }
            if(spi_rx>0){
                printf("spi_rx reads: %u\n", spi_rx);
                spi_rx = 0;
            }
        }

    }

}
