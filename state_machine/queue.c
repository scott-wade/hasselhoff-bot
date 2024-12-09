#include "queue.h"
#include "state_machine_sub.h"
#include "globals.h"

simple_queue_t simpleQ;

void init_simple_queue(device_enum device)
{
    simpleQ.device = device;
    simpleQ.size = 0;
    simpleQ.insert_indx = 0;
    simpleQ.remove_indx = 0;
}

void insert_to_simple_queue(sub_events_t event)
{

    simpleQ.events[simpleQ.insert_indx] = event;
    printf("Event %d added to index %d\n", 
            event.type, simpleQ.insert_indx);
    simpleQ.insert_indx++;
    if (simpleQ.insert_indx == MAX_ELEMENTS)
    {
        simpleQ.insert_indx = 0;
        printf("Resetting insert_indx");
    }
    simpleQ.size++;
}

sub_events_t pop_from_simple_queue(void){
    /* Get the first item in the simple queue */
    if (isempty_simple_queue()){
        printf("simpleQ is empty but tried to pop anyway\n");
    }else{
        sub_events_t current_event = simpleQ.events[simpleQ.remove_indx];
        simpleQ.remove_indx ++;
        if (simpleQ.insert_indx == MAX_ELEMENTS){
                simpleQ.remove_indx = 0;
            }
        simpleQ.size --;
        return current_event;
    }
}

uint8_t isempty_simple_queue(void){
    /* Return 1 if queue is empty, 0 if not */
    return simpleQ.size == 0;

}

uint8_t isfull_simple_queue(void){
    /* Return 1 if queue is full, 0 if not */
    if(simpleQ.size >= MAX_ELEMENTS){
        return 1;
    }else{
        return 0;
    }
}
