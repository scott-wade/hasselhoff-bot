/**
  ******************************************************************************
  * @file    state_machine_spi.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for SPI
  ******************************************************************************
  */


#include "spi_queue.h"
#include "state_machine_sub.h"
#include "../hardware/hardware_stm_spi.h"
#include "../hardware/hardware_stm_gpio.h"
#include "../hardware/hardware_stm_timer.h"
#include <cstdint>
#include "../globals.h"
#include "../applications/comms.h"
#include "state_machine_SPI.h"

// macros
#define ACK_PACKET (uint8_t)(2)
#define CLOCK_TIMER 5

// global variables for state machine
Queue* SPI_COMMS_EVENT_QUEUE;
Queue* SPI_SENSOR_EVENT_QUEUE;
transmitEvent CURRENT_COMMS_TRANSMIT_EVENT;

transmitEvent CURRENT_SENSOR_TRANSMIT_EVENT;
Queue* SPI_COMMS_RECIEVED_QUEUE;
Queue* SPI_SENSOR_RECIEVED_QUEUE;
uint8_t SPI_COMMS_STATE = 0; // 0 for IDLE, i for CSi
uint8_t SPI_SENSOR_STATE = 0; // 0 for IDLE, i for CSi


void init_state_machine_spi(Spi_State_Machine_t spi_type){
    // // initialize timer5 in upcounting mode w/ ticks at 1 MHz (ie 1 us res)
    // // arr calibrated for timer to wrap every 20ms
    // initTimer(CLOCK_TIMER, 89, 20000);
    // // enable the timer
    // enableTimer(CLOCK_TIMER);
    switch(spi_type){
        case NUCLEO_PARENT: // initialize a parent on SPI1
            configureSPIPeripheral(SPI_PARENT, 1);
            SPI_COMMS_EVENT_QUEUE = createQueue(sizeof(transmitEvent));
            SPI_COMMS_RECIEVED_QUEUE = createQueue(sizeof(uint8_t));
            CURRENT_COMMS_TRANSMIT_EVENT.txQueue = createQueue(sizeof(uint16_t));
        break;
        case NUCLEO_CHILD: // initialize a child on SPI1
            configureSPIPeripheral(SPI_CHILD, 1);
            SPI_COMMS_EVENT_QUEUE = createQueue(sizeof(transmitEvent));
            SPI_COMMS_RECIEVED_QUEUE = createQueue(sizeof(uint8_t));
            CURRENT_COMMS_TRANSMIT_EVENT.txQueue = createQueue(sizeof(uint16_t));
        break;
        case SENSOR_PARENT: // initialize a parent on SPI4
            configureSPIPeripheral(SPI_PARENT, 4);
            SPI_SENSOR_EVENT_QUEUE = createQueue(sizeof(transmitEvent));
            SPI_SENSOR_RECIEVED_QUEUE = createQueue(sizeof(uint8_t));
            CURRENT_SENSOR_TRANSMIT_EVENT.txQueue = createQueue(sizeof(uint16_t));
        break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;

    }
    
}



void event_handler_spi(Spi_State_Machine_t spi_type){
    /* Checks and handles events for spi */
    // state machine local vars
    static uint32_t* read_var_addr = NULL;
    
    uint8_t spi_id;
    if(spi_type == SENSOR_PARENT) spi_id = 4;
    else spi_id = 1;
    // check for events
    switch(spi_type){
        case NUCLEO_PARENT: // parent on SPI1
            if (!isEmpty(SPI_COMMS_EVENT_QUEUE)){
                CURRENT_COMMS_TRANSMIT_EVENT = *(transmitEvent*)dequeue(SPI_COMMS_EVENT_QUEUE);
                printf("Dequeued spi comms event \n");
            }
        break;
        case NUCLEO_CHILD: // child on SPI1
            if (!isEmpty(SPI_COMMS_EVENT_QUEUE)){
                CURRENT_COMMS_TRANSMIT_EVENT = *(transmitEvent*)dequeue(SPI_COMMS_EVENT_QUEUE);
            }
        break;
        case SENSOR_PARENT: // parent on SPI4
            if (!isEmpty(SPI_SENSOR_EVENT_QUEUE)){
                CURRENT_SENSOR_TRANSMIT_EVENT = *(transmitEvent*)dequeue(SPI_SENSOR_EVENT_QUEUE);
            }
        break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }

    // check for timeout event
    // TODO


    // Get the most recent transmit event from the queue
    transmitEvent currentEvent;
    uint8_t state;
    switch(spi_type){
        case NUCLEO_PARENT: {
            currentEvent = CURRENT_COMMS_TRANSMIT_EVENT;
            state = SPI_COMMS_STATE;} break;
        case NUCLEO_CHILD: {currentEvent = CURRENT_COMMS_TRANSMIT_EVENT;
            state = SPI_COMMS_STATE;} break;
        case SENSOR_PARENT: {currentEvent = CURRENT_SENSOR_TRANSMIT_EVENT;
            state = SPI_SENSOR_STATE;} break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }

    if((!isEmpty(currentEvent.txQueue)) && (state == 0)){// begin transmission
        // state transition
        if (currentEvent.child_id > 3){
            fprintf(stderr, "Requested unsupported SPI child %u\n", currentEvent.child_id);
        }else{
            state = currentEvent.child_id;
        }
        switch(spi_type){ // update global state variable
            case NUCLEO_PARENT: {SPI_COMMS_STATE = state;} break;
            case NUCLEO_CHILD: {SPI_COMMS_STATE = state;} break;
            case SENSOR_PARENT: {SPI_SENSOR_STATE = state;} break;
            default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
        }

        // clear CSi pin if addressing sensors
        if (spi_type == SENSOR_PARENT) 
            SETorCLEARGPIOoutput(CS_PINS[state/2], CS_PINS[state/2+1], 0);

        // write to TX buffer
        uint16_t currpacket = *(uint16_t*)dequeue(currentEvent.txQueue);
        printf("writing to tx: %u\n", currpacket);
        writeTX(spi_id, currpacket);
        // enable TXE interrupt
        enableSpiTXEInterrupts(spi_id);
    }
    
}


void requestSpiTransmit(Spi_State_Machine_t spi_type, uint8_t child_id, uint16_t packet, uint32_t* read_var_addr){
    /* Applications will call this function to start a single SPI transaction */
    //printf("requesting spi transmission\n");

    transmitEvent testEvent;
    testEvent.txQueue = createQueue(sizeof(uint16_t)); 
    enqueue(testEvent.txQueue, &packet); //TODO change this to add support for multi packet comms
    testEvent.child_id = child_id;
    testEvent.read_var_addr = read_var_addr;
    
    switch(spi_type){// enqueue to the proper state machine's transmit queue
        case NUCLEO_PARENT: {enqueue(SPI_COMMS_EVENT_QUEUE, &testEvent);} break;
        case NUCLEO_CHILD: {enqueue(SPI_COMMS_EVENT_QUEUE, &testEvent);} break;
        case SENSOR_PARENT: {enqueue(SPI_SENSOR_EVENT_QUEUE, &testEvent);} break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }
    
}


void spiInterruptHandler(uint8_t spi_id){
    // handle TXE=1 and RXNE=1 interrupts

    // define masks
    uint16_t TXE_MASK = 0b10;
    uint16_t RXNE_MASK = 0b1;

    // set local vars
    uint8_t* stateptr;
    Queue* transmitQueue;
    Queue* receivedQueue;
    if(spi_id == 1) {
        stateptr = &SPI_COMMS_STATE;
        transmitQueue = CURRENT_COMMS_TRANSMIT_EVENT.txQueue;
        receivedQueue = SPI_COMMS_RECIEVED_QUEUE;
    }
    else if (spi_id == 4) {
        stateptr = &SPI_SENSOR_STATE;
        transmitQueue = CURRENT_SENSOR_TRANSMIT_EVENT.txQueue;
        receivedQueue = SPI_SENSOR_RECIEVED_QUEUE;
    }

    // check to see if it is a transmit or recieve event
    uint16_t current_status_register = readSpiStatusRegister(spi_id);

    // check recieve event first
    if ((current_status_register & RXNE_MASK) > 0){

        if (*stateptr == 0){// if state == IDLE
            //readRX() and set according event flags
            uint16_t recievedData = readRX(spi_id);
            uint8_t first8bits = *((uint8_t*)&(recievedData)+1);
            uint8_t last8bits = *((uint8_t*)&(recievedData)+0);
            enqueue(receivedQueue, &first8bits);
            enqueue(receivedQueue, &last8bits);

            //TODO implement the event flag setting according to Sidney's state machine

            //writeTX(acknowledgement packet)"
            writeTX(spi_id, ACK_PACKET);

        }else{// if state not IDLE
            // RecieveQueue.append(readRX())
            uint8_t recievedData = (uint8_t)readRX(spi_id);
            enqueue(receivedQueue, &recievedData);
            if (isEmpty(transmitQueue)){// If transmitQueue.empty:
                // deactivate (set high) all cs pins
                if(spi_id == 4){
                    for(int i = 0; i < 3; i++){
                        SETorCLEARGPIOoutput(CS_PINS[i/2], CS_PINS[i/2+1], 1);
                    }
                }
                // Transition to IDLE
                *stateptr = 0;
                // TODO Stop timeout timer
            }            
        }
        
    }
        

    // if transmit event
    if ((current_status_register & TXE_MASK) > 0){
        printf("Handling TXE interrupt\n");
        if (*stateptr == 0){ // if state == IDLE
            // disable Spi TXE Interrupts(SPI id)
            disableSpiTXEInterrupts(spi_id);
        }else if (*stateptr > 0){// if state == TXi
            // TODO Start Timeout timer
            if (!isEmpty(transmitQueue)){// If TransmitQueue not empty:
                uint16_t packet = *(uint16_t*)dequeue(transmitQueue);
                writeTX(spi_id, packet);
            }else{// If TransmitQueue empty, disable TXE interrupt in SPIx_CR2
                disableSpiTXEInterrupts(spi_id);
            }
        }

    }
    
}

void SPI1_IRQHandler(void){
    // handle the interrupt events for SPI1

    // call spiInterruptHandler(1);
    spiInterruptHandler(1);

}

void SPI4_IRQHandler(void){
    // handle the interrupt events for SPI4

    // call spiInterruptHandler(4);
    spiInterruptHandler(4);
}