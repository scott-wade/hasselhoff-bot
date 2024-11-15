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
#include "../debug_mort.h"


void testReadRegOpMode(void){
    /* Try to read the current RegOpMode settings register from the sx1278 */ 
    configureSPIParent(1);
    

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
    configureSPIParent(1);
    // configure gpio port for CS

    uint8_t wnr = 1; // write
    uint8_t data = (uint8_t)(0b1 << 7) | (uint8_t)(0b001);
    uint8_t addr = 0x01;
    uint8_t addr_packed = (uint8_t)(wnr << 7 | addr);
    uint16_t packet = (uint16_t)(addr_packed << 8 | data);
    printf("packet: %u\n", packet);

    // set CS pin A4 low
    SETorCLEARGPIOoutput(0, 4, 0);
    writeTX(1, packet);

    // set CS pin A4 high
    SETorCLEARGPIOoutput(0, 4, 1);
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