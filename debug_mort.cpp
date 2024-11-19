#include "mbed.h"
#include "debug_mort.h"

void debugprint(uint16_t number)
{
    printf("Got to %u\n",number);    
}

void debugprintHelloWorld( void )
{
    printf("I'm aliiiiiivveeee!!!\n");
} 

void delay(int num)
{
    for (int i=0; i<num*1000000; i++){
        // busy wait
    }
}