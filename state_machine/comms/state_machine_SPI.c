/**
  ******************************************************************************
  * @file    state_machine_spi.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November -2024
  * @brief   State Machine for SPI
  ******************************************************************************
  */


#include "packet.h"
#include "spi_queue.h"
#include "queue.h"
#include "state_machine_sub.h"
#include "hardware_stm_spi.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_timer.h"
#include <cstdint>
#include "globals.h"
#include "state_machine_SPI.h"
#include <stdbool.h>

// macros
#define ACK_PACKET (uint8_t)(0b10101010)
#define CLOCK_TIMER 5

// global variables for state machine
Queue SPI_COMMS_EVENT_QUEUE;
Queue SPI_SENSOR_EVENT_QUEUE;
transmitEvent CURRENT_COMMS_TRANSMIT_EVENT;
transmitEvent CURRENT_SENSOR_TRANSMIT_EVENT;
uint8_t SPI_COMMS_STATE = 99; // 99 for IDLE, i for CSi
uint8_t SPI_SENSOR_STATE = 99; // 99 for IDLE, i for CSi


void init_state_machine_spi(Spi_State_Machine_t spi_type){
    // // initialize timer5 in upcounting mode w/ ticks at 1 MHz (ie 1 us res)
    // // arr calibrated for timer to wrap every 20ms
    // initTimer(CLOCK_TIMER, 89, 20000);
    // // enable the timer
    // enableTimer(CLOCK_TIMER);
    switch(spi_type){
        case NUCLEO_PARENT: // initialize a parent on SPI1
            configureSPIPeripheral(SPI_PARENT, 1);
            SPI_COMMS_EVENT_QUEUE = createQueue();
        break;
        case NUCLEO_CHILD: // initialize a child on SPI1
            configureSPIPeripheral(SPI_CHILD, 1);
            SPI_COMMS_EVENT_QUEUE = createQueue();
        break;
        case SENSOR_PARENT: // initialize a parent on SPI4
            configureSPIPeripheral(SPI_PARENT, 4);
            SPI_SENSOR_EVENT_QUEUE = createQueue();
        break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;

    }
    
}



void event_handler_spi(Spi_State_Machine_t spi_type){
    /* Checks and handles events for spi */
    uint8_t newTransmission = 0;
    
    // select the SPI id for the current spi type
    uint8_t spi_id;
    if(spi_type == SENSOR_PARENT) spi_id = 4;
    else spi_id = 1;
    // check for events
    switch(spi_type){
        case NUCLEO_PARENT: // parent on SPI1
            //printf("SPI_COMMS_EVENT_QUEUE has size %u \n", SPI_COMMS_EVENT_QUEUE.size);
            if (!isEmpty(&SPI_COMMS_EVENT_QUEUE) && SPI_COMMS_STATE == 99){
                CURRENT_COMMS_TRANSMIT_EVENT = dequeue(&SPI_COMMS_EVENT_QUEUE);
                // printf("Dequeued spi comms event \n");
                newTransmission = 1;
            }
        break;
        case NUCLEO_CHILD: // child on SPI1
            if (!isEmpty(&SPI_COMMS_EVENT_QUEUE) && SPI_COMMS_STATE == 99){
                CURRENT_COMMS_TRANSMIT_EVENT = dequeue(&SPI_COMMS_EVENT_QUEUE);
                newTransmission = 1;
            }
        break;
        case SENSOR_PARENT: // parent on SPI4
            if (!isEmpty(&SPI_SENSOR_EVENT_QUEUE) && SPI_SENSOR_STATE == 99){
                CURRENT_SENSOR_TRANSMIT_EVENT = dequeue(&SPI_SENSOR_EVENT_QUEUE);
                //printf("Dequeued spi sensor comm event \n");
                newTransmission = 1;
            }
        break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }

    // check for timeout event
    // TODO

    /* Process events */

    // Select which event type we are processing
    transmitEvent currentEvent;
    uint8_t currentState;
    switch(spi_type){
        case NUCLEO_PARENT: {currentEvent = CURRENT_COMMS_TRANSMIT_EVENT;
            currentState = SPI_COMMS_STATE;} break;
        case NUCLEO_CHILD: {currentEvent = CURRENT_COMMS_TRANSMIT_EVENT;
            currentState = SPI_COMMS_STATE;} break;
        case SENSOR_PARENT: {currentEvent = CURRENT_SENSOR_TRANSMIT_EVENT;
            currentState = SPI_SENSOR_STATE;} break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }

    if((newTransmission == 1) && (currentState == 99)){// begin transmission if idle and new tx event
        // state transition
        if (currentEvent.child_id > 3){
            fprintf(stderr, "Requested unsupported SPI child %u\n", currentEvent.child_id);
        }else{
            currentState = currentEvent.child_id;
        }
        switch(spi_type){ // update global state variable
            case NUCLEO_PARENT: {SPI_COMMS_STATE = currentState;} break;
            case NUCLEO_CHILD: {SPI_COMMS_STATE = currentState;} break;
            case SENSOR_PARENT: {SPI_SENSOR_STATE = currentState;} break;
            default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
        }

        // clear CSi pin if addressing sensors
        if (spi_type == SENSOR_PARENT) {
            SETorCLEARGPIOoutput(CS_PINS[currentState*2], CS_PINS[currentState*2+1], 0);
            //printf("CS pin low \n");
        }
        // write to TX buffer
        // printf("writing to tx: %u\n", currpacket);
        printf("Writing to TX with packet: %u\n",
            currentEvent.tx_packet);
        writeTX(spi_id, currentEvent.tx_packet);
        // enable TXE interrupt
        enableSpiTXEInterrupts(spi_id);
    }
    
}


void requestSpiTransmit(Spi_State_Machine_t spi_type, uint8_t child_id, uint16_t packet, uint8_t* read_var_addr){
    /* Applications will call this function to start a single SPI transaction */
    //printf("requesting spi transmission\n");

    transmitEvent testEvent;
    testEvent.tx_packet = packet;
    testEvent.child_id = child_id;
    testEvent.read_var_addr = read_var_addr;
    
    switch(spi_type){// enqueue to the proper state machine's transmit queue
        case NUCLEO_PARENT: {enqueue(&SPI_COMMS_EVENT_QUEUE, testEvent);} break;
        case NUCLEO_CHILD: {enqueue(&SPI_COMMS_EVENT_QUEUE, testEvent);} break;
        case SENSOR_PARENT: {enqueue(&SPI_SENSOR_EVENT_QUEUE, testEvent);} break;
        default: printf(stderr, "Unsupported/incorrect SPI Machine type"); break;
    }
    
}

sub_event_type_t packetToSubEvent(packet_type_t msg_type){
    /* Convert packet headers to submarine event types*/
    sub_event_type_t subReceivedEvent;
    switch (msg_type){
        case RESET_MSG: subReceivedEvent = RESET_MSG_RECEIVED; break;
        case DRIVE_FB_MSG: subReceivedEvent = DRIVE_MSG_FB_RECEIVED; break;
        case DRIVE_LR_MSG: subReceivedEvent = DRIVE_MSG_LR_RECEIVED; break;
        case DRIVE_DS_MSG: subReceivedEvent = DRIVE_MSG_DS_RECEIVED; break;
        case LAND_MSG: subReceivedEvent = LAND_MSG_RECEIVED; break;
        case STATUS_REQ_MSG: subReceivedEvent = STATUS_REQUEST_RECEIVED; break;
        default: printf(stderr, "Invalid packet header"); break;
    }

    return subReceivedEvent;
}

void spiInterruptHandler(uint8_t spi_id){
    // handle TXE=1 and RXNE=1 interrupts

    // define masks
    uint16_t TXE_MASK = 0b10;
    uint16_t RXNE_MASK = 0b1;

    // check to see if it is a transmit or recieve event
    uint16_t current_status_register = readSpiStatusRegister(spi_id);

    // Transition to IDLE
    
    uint8_t state;
    if(spi_id == 1) {
        state = SPI_COMMS_STATE;
    }
    else if (spi_id == 4) {
        state = SPI_SENSOR_STATE = 99;
    }

    // check recieve event first
    if ((current_status_register & RXNE_MASK) > 0){

        if (state == 99){// if state == IDLE
            // writeTX(spi_id, (uint8_t)170);
            //readRX() and enqueue according event to the submarine queue
            uint16_t recievedData = readRX(spi_id);
            // printf("Received data %u\n\n", recievedData);
            uint8_t first8bits = *((uint8_t*)&(recievedData)+1);
            uint8_t last8bits = *((uint8_t*)&(recievedData)+0);
            // construct the sub state machine event
            sub_events_t receivedEvent;
            receivedEvent.type = packetToSubEvent((packet_type_t)first8bits);
            receivedEvent.data = last8bits;

            // insert the event to the sub state machine event queue
            // ignoring status request mesages
            if(receivedEvent.type != STATUS_REQUEST_RECEIVED)
                insert_to_simple_queue(receivedEvent);
            // printf("Adding event\n");
            

        }
        else {// if currently transmitting
            // (this is the typical conclusion of a message for sensor communication)
            // write received data to the read_var_addr of the current transmission event
            uint8_t receivedData = 0;
            receivedData = (uint8_t)readRX(spi_id);
            // reading in from a sensor event
            if(spi_id == 4) {
                *(CURRENT_SENSOR_TRANSMIT_EVENT.read_var_addr) = receivedData;
                // raise the CS pin since we're done reading
                uint8_t childID = CURRENT_SENSOR_TRANSMIT_EVENT.child_id;
                SETorCLEARGPIOoutput(CS_PINS[childID*2], CS_PINS[childID*2+1], 1);
                //printf("received: %hhu \n", receivedData);
            }
            // reading in from a nucleo
            else {
                *(CURRENT_COMMS_TRANSMIT_EVENT.read_var_addr) = receivedData;                
            }
            // Transition to IDLE
            if(spi_id == 1) {
                SPI_COMMS_STATE = 99;
            }
            else if (spi_id == 4) {
                SPI_SENSOR_STATE = 99;
            }
            // TODO Stop timeout timer
            // printf("Processed confirmation with recieved data: %u", recievedData);
        }
        
    }
        

    // if transmit event
    if ((current_status_register & TXE_MASK) > 0){
        //printf("Handling TXE interrupt\n");
        if (state == 99){ // if state == IDLE

            // disable Spi TXE Interrupts(SPI id)
            disableSpiTXEInterrupts(spi_id);
        }else{// if state == TXi, end the transmission
            // TODO Start Timeout timer
            disableSpiTXEInterrupts(spi_id);
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


/** Wrapper function for remote to call requestSpiTransmit()
 * @param msg_type: type of the message
 * @param packet: data to sent
 * @param read_var_addr: address to store the return value (only for messages that have a response)
 */
void requestSpiTransmit_remote(packet_type_t msg_type, uint8_t data, uint8_t* read_var_addr) {
    uint8_t header = (uint8_t)msg_type;

    // Packet is first 8 bits is message type and last 8 bits is the data
    uint16_t packet = (header << 8) | data;

    requestSpiTransmit(NUCLEO_PARENT, 0, packet, read_var_addr);

    // Print log messages
    bool debug = false;
    if (debug) {
        printf("SPI type: ");
        switch (msg_type){
            case RESET_MSG: printf("RESET_MSG"); break;
            case DRIVE_FB_MSG: printf("DRIVE_FB_MSG"); break;
            case DRIVE_LR_MSG: printf("DRIVE_LR_MSG"); break;
            case DRIVE_DS_MSG: printf("DRIVE_DS_MSG"); break;
            case LAND_MSG: printf("LAND_MSG"); break;
            case STATUS_REQ_MSG: printf("STATUS_REQ_MSG"); break;
            default: printf(stderr, "Invalid packet header"); break;
        }
        printf(", data: %d\n", data);
    }
}

