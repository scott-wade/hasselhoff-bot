/**
  ******************************************************************************
  * @file    state_machine_spi.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for SPI
  ******************************************************************************
  */


#include "state_machine_sub.h"
#include "../hardware/hardware_stm_spi.h"
#include "../hardware/hardware_stm_gpio.h"
#include <cstdint>
#include "../globals.h"

// global variables for state machine
uint16_t* SPI_READ_ADDR = NULL; // address to send current RX to
uint32_t SPI_OUTGOING_PACKET; // packet to send in current transaction
uint8_t SPI_TRANSMIT_FLAG = 0;

void init_state_machine_spi(void){
    /* Initialize sub state machine */ 
    configureSPIParent(1);
    // i think the GPIO config stuff should go here and not in configureSPIParent

}


void event_handler_spi(void){
    /* Checks and handles events for spi */
    // state machine local vars
    static uint8_t state = 0; // 0 for IDLE, i for CSi
    static uint8_t cs_pins[10]; // support 3 devices for now
    cs_pins[0] = 0; // CS1 is pin A4
    cs_pins[1] = 4; // CS1 is pin A4

    // check for events

    // check for init_tx flag
    
    if((SPI_TRANSMIT_FLAG > 0) && (state == 0)){// begin transmission
        // state transition
        if (SPI_TRANSMIT_FLAG > 3){
            fprintf(stderr, "Requested unsupported SPI child %u", SPI_TRANSMIT_FLAG);
        }else{
            state = SPI_TRANSMIT_FLAG;
        }
        // clear CSi pin
        SETorCLEARGPIOoutput(cs_pins[state/2], cs_pins[state/2+1], 0);
        // write to TX buffer
        writeTX(1, SPI_OUTGOING_PACKET);       
        printf("Handling transmission event");
        SPI_TRANSMIT_FLAG = 0;
    }

    // handle SPI status register
    uint16_t spi1_status_register = readSpiStatusRegister(1);

    // receieved packet handling
    if(((spi1_status_register & (uint16_t)0b1) > 0) && (state > 0)){ // packet recieved and not IDLE
        if(SPI_READ_ADDR != NULL) *SPI_READ_ADDR = readRX(1);
        SETorCLEARGPIOoutput(cs_pins[state/2], cs_pins[state/2+1], 1);
        state = 0;
        resetSpiRXNE(1);
    }

    
}


void requestSpiTransmit(uint8_t child_id, uint16_t packet, uint16_t* read_var_addr){
    /* Applications will call this function to start a single SPI transaction */
    printf("requesting spi transmission\n");
    // set transmit flag
    SPI_TRANSMIT_FLAG = child_id;
    SPI_OUTGOING_PACKET = packet;
    SPI_READ_ADDR = read_var_addr;
}




void SPI1_IRQHandler(void){
    // handle the interrupt events for SPI1

    // define the address in memory for the SPI1 SR


}

void SPI4_IRQHandler(void){
    // handle the interrupt events for SPI4



}