#include "hardware_stm_gpio.h"

void testB0Set(void){
    // turn LED1 on 
    initGPIOasMode(1, 0, 1, 0, 0, 1, 0);
    SETorCLEARGPIOoutput(1,0,1);
}

void testB0Clear(void){
    // turn LED1 off
    initGPIOasMode(1, 0, 1, 0, 0, 1, 0);
    SETorCLEARGPIOoutput(1,0,0);
}