/**
  ******************************************************************************
  * @file    test_spi.c 
  * @author  scottwad@andrew.cmu.edu
  * @version 1.0
  * @date    November 2024
  * @brief   functions for testing SPI using an sx1276 transceiver
  ******************************************************************************
  */


#include "../hardware/hardware_stm_gpio.h"
#include "../hardware/hardware_stm_spi.h"
#include "../state_machine/state_machine_SPI.h"
#include "../debug_mort.h"
#include "../state_machine/spi_queue.h"
#include <cstdint>
#include "../state_machine/state_machine_sub.h"
#include "../applications/packet.h"
#include "globals.h"


void testReadRegOpMode(void){
    /* Try to read the current RegOpMode settings register from the sx1278 */ 
    configureSPIPeripheral(SPI_PARENT,1);
    

    uint8_t wnr = 0; // read
    uint8_t data = (uint8_t)(0b1 << 7);
    uint8_t addr = 0x01;
    uint8_t addr_packed = (uint8_t)(wnr << 7 | addr);
    uint16_t packet = (uint16_t)(addr_packed << 8 | data);
    printf("packet: %u\n", packet);
    // set CS pin A4 low
    SETorCLEARGPIOoutput(0, 4, 0);
    writeTX(1, packet);
    
    // check status of txe bit
    printf("status register: %u \n", readSpiStatusRegister(1));

    // set CS pin A4 high
    SETorCLEARGPIOoutput(0, 4, 1);

    uint16_t returnvalue = readRX(1);
    debugprint(returnvalue); // this should hopefully return something
}

// hypothesis: this will work, but there is a risk that the 
// child will not ever consider this transaction finished
// or it might wait too long to consider this transaction finished
// because we are not setting CS to high precisely when we finish transmission

// the first (chronological) 8 bits of the RX are probably garbo 
// bits 0-7 of RX are supposed to be the current contents of that register

void testReadWriteRegOpMode(void){
    /* Try to write to the RegOpMode settings register from the sx1278 */ 
    configureSPIPeripheral(SPI_PARENT,1);
    // configure gpio port for CS

    uint8_t wnr = 1; // write
    uint8_t data = (uint8_t)(0b1 << 7) | (uint8_t)(0b001);
    uint8_t addr = 0x01;
    uint8_t addr_packed = (uint8_t)(wnr << 7 | addr);
    uint16_t packet = (uint16_t)(addr_packed << 8 | data);
    printf("packet: %u\n", packet);

    // set child select pin A4 low
    SETorCLEARGPIOoutput(0, 4, 0);

    // write to our TX buffer
    writeTX(1, packet);

    // set child select pin A4 high
    SETorCLEARGPIOoutput(0, 4, 1);

    // read from our RX buffer
    uint16_t returnvalue = readRX(1);
    printf("Read value from register %u \n",returnvalue);
    

    /* wrote a value and printed old register value, now let's see if it changed */

    wnr = 0; // read
    addr_packed = (uint8_t)(wnr << 7 | addr);
    packet = (uint16_t)(addr_packed << 8 | data);
    printf("addr packed for read packet: %u \n", addr_packed);
    printf("read packet: %u\n", packet);

    // set CS pin A4 low
    SETorCLEARGPIOoutput(0, 4, 0);

    writeTX(1, packet);
    // set CS pin A4 high
    SETorCLEARGPIOoutput(0, 4, 1);

    returnvalue = readRX(1);
    printf("Read value from register after write: %u \n",returnvalue); // this should hopefully print the same as packet


}


void testSPIStateMachine(void){
    /* should print rx = 4*/ 

    // initialize SPI state machine
    init_state_machine_spi(1);

    // init debugging packet 
    uint8_t wnr = 0; // read
    uint8_t data = (uint8_t)(0b1 << 7);
    uint8_t addr = 0x01;
    uint8_t addr_packed = (uint8_t)(wnr << 7 | addr);
    uint16_t packet = (uint16_t)(addr_packed << 8 | data);
    printf("packet: %u\n", packet);

    // var to read from spi to
    uint8_t spi_rx = 0;

    // run state machine
    int delaything = 0;

    while(1){
        event_handler_spi(NUCLEO_PARENT);
        delaything = delaything + 1;
        if(delaything == 100000){
            requestSpiTransmit(NUCLEO_PARENT, 1, packet, &spi_rx);
            delaything=0;
        }
        if(spi_rx>0){
            printf("spi_rx reads: %u\n", spi_rx);
            spi_rx = 0;
        }
    }
}


void testSPIQueue(void){
    Queue* testQueue = createQueue(sizeof(transmitEvent));

    // create a test event
    uint8_t returnvalue = 0;
    transmitEvent testEvent;
    testEvent.tx_packet = 0;
    testEvent.child_id = 47;
    testEvent.read_var_addr = &returnvalue;

    // add that event to the queue
    enqueue(testQueue, &testEvent);

    if(!isEmpty(testQueue)){
        printf("testQueue is not Empty (PASS)\n");
    }else{
        printf("testQueue is Empty (BAD)\n");
    }

    // try to retrieve the event from the queue

    transmitEvent returnedEvent = *(transmitEvent*)dequeue(testQueue);
    printf("Want 47, got child id %u\n", returnedEvent.child_id); // should get 47

}

void testNucleoTransmitting(){
    // send periodic STATUS_REQ_MSG packets (header=0xde) with 0xad as the data value

    init_state_machine_spi(NUCLEO_PARENT);
    

    uint32_t iter = 0;
    uint8_t read_var = 99;
    uint8_t read_var_prev = read_var;

    for (int i = 0; i < 100000000; i++){
        printf("Read value: %u\n", read_var);
        delay(1);
        if((iter % 5) == 0){
            requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &read_var);
            printf("Requesting Status Transmission, type %u \n", STATUS_REQ_MSG);
        }
        if(((iter+1) % 5) == 0){
            requestSpiTransmit_remote(RESET_MSG, 0, NULL);
            printf("Requesting Reset Transmission %u \n", RESET_MSG);
        }
        if(((iter+1) % 5) == 0){
            requestSpiTransmit_remote(DRIVE_DS_MSG, 212, NULL);
            printf("Requesting Drive DS Transmission %u \n", DRIVE_DS_MSG);
        }
        // if(((iter+2) % 5) == 0){
        //     requestSpiTransmit_remote(STATUS_REQ_MSG, 0, &read_var);
        //     printf("Requesting Status Transmission\n");
        // }
        event_handler_spi(NUCLEO_PARENT);
        
        iter ++;
    }

}

void testNucleoReceiving(){
    /* Process sub-side events recieved via SPI */ 
    sub_states_t debug_sub_state_state = WELCOME;
    uint8_t debug_sub_IR = 1;

    init_state_machine_spi(NUCLEO_CHILD);
    init_sub_debugging(debug_sub_state_state, debug_sub_IR);

    // print the packet that the sub is sending to the remote
    printf("Sensor message to send to remote: %u\n", SUBMARINE_CURRENT_STATUS_MSG);

    // writeTX(1, (uint8_t)85);
    writeTX(1, SUBMARINE_CURRENT_STATUS_MSG);
    while(1){
        // service spi state machine
        event_handler_spi(NUCLEO_CHILD);
        event_handler_sub();
        
    }

}


