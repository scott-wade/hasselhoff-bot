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
#include "../applications/comms.h"
#include "state_machine_SPI.h"

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
    static uint8_t cs_pins[6]; // support 3 devices for now
    cs_pins[0] = 0; // CS1 is pin A15
    cs_pins[1] = 15; // CS1 is pin A15

    // check for events

    // check for init_tx flag
    
    if((SPI_TRANSMIT_FLAG > 0) && (state == 0)){// begin transmission
        // state transition
        if (SPI_TRANSMIT_FLAG > 3){
            fprintf(stderr, "Requested unsupported SPI child %u\n", SPI_TRANSMIT_FLAG);
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


void spiInterruptHandler(uint8_t spi_id){
    // check to see if it is a transmit or recieve event

    // if transmit event
        // if state == IDLE
            // disableSpiTXEInterrupts(SPI id)
        // if state == TXi
            // Start Timeout timer
            // If TransmitQueue not empty:
                // writeTX(TransmitQueue.pop())
                // Reset TXE interrupt
            // If TransmitQueue empty, disable TXE interrupt in SPIx_CR2

    // if recieve event
        // if state == IDLE
            //readRX() and set according event flags
            //writeTX(acknowledgement packet)"
        // if state not IDLE
            // RecieveQueue.append(readRX())
            // clear RXNE
            // If TransmitQueue.empty:
                // setGPIO(CSi pin) if necessary
                // Transition to IDLE
                // Stop timeout timer
                // *SPI_current_read_to = RecieveQueue.asint"
}

void SPI1_IRQHandler(void){
    // handle the interrupt events for SPI1

    // define the address in memory for the SPI1 SR


    // call spiInterruptHandler(1);
    

}

void SPI4_IRQHandler(void){
    // handle the interrupt events for SPI4

    // check to see if it is a transmit or recieve event

    // call spiInterruptHandler(4);
}