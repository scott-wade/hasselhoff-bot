#include "ir_range.h"
#include "test_ir.h"

void test_ir()
{
    initIRSensor(0);
    while(1)
    {
        uint8_t tripped = getSensorTripped();
        if (tripped)
        {
            printf("Tripped!!!\n");
        } else {
            printf("Clear\n");
        }
        delay(10);
        
    }
}