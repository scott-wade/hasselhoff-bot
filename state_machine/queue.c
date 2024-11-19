#include "queue.h"
#include "state_machine_sub.h"

void init_simple_queue(device_enum device)
{
    simpleQ.device = device;
    simpleQ.size = 0;
    simpleQ.insert_indx = 0;
    simpleQ.process_indx = 0;
}

void insert_to_simple_queue(sub_events_t event)
{
    simpleQ.events[simpleQ.insert_indx].sub_event = event;
    simpleQ.insert_indx++;
    if (simpleQ.insert_indx == MAX_ELEMENTS)
    {
        simpleQ.insert_indx = 0;
    }
}
