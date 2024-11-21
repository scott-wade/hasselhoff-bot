#include "queue.h"
#include "state_machine_sub.h"

void init_simple_queue(simple_queue_t* simpleQ, device_enum device)
{
    simpleQ->device = device;
    simpleQ->size = 0;
    simpleQ->insert_indx = 0;
    simpleQ->process_indx = 0;
}

void insert_to_simple_queue(simple_queue_t* simpleQ, uint8_t event)
{

    simpleQ->events[simpleQ->insert_indx] = event;
    printf("Event %d added to index %d\n", 
            event, simpleQ->insert_indx);
    simpleQ->insert_indx++;
    if (simpleQ->insert_indx == MAX_ELEMENTS)
    {
        simpleQ->insert_indx = 0;
        printf("Resetting insert_indx");
    }
    simpleQ->size++;
}
