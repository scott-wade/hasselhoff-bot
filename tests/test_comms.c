#include "test_comms.h"


void test_comms(void)
{
    init_sub();
    sub_events_t event;
    printf("Enter command");
    while(1)
    {
        scanf("%d\n", &event);
        insert_to_simple_queue(event);
        event_handler_sub();

    }
}