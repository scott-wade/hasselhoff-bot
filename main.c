#include <cstdint>
#include "main.h"
#include "debug_mort.h"
#include "test_ir.h"
#include "tests/test_comms.h"


#define WHICH_NUCLEO 2 //change for compilation, 0 for remote, 1 for submarine, 2 for debug
#define SUB

int main(void){
    test_comms();
    // test_ir();
    while(1) {}
}


void delay(uint16_t ms)
{
    for(int i =0;i<ms;i++)
    {
        for(int i = 0; i < ms; i++)
        {

        }
    }
}