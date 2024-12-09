#include "test_sub_state_machine.h"
#include <assert.h>

void test_sub_state_machine(void)
{
    sub_events_t event;
    init_sub(); // Initialise sub hardware, sensor and state machine
    assert(subState.state == IDLE);
    assert(simpleQ.size == 0);

    // Test case 1: No events
    event_handler_sub();
    // Expected result Nothing?
    assert(subState.state == IDLE);
    printf("TEST Case 1 Passed\n\n");

    // Test case 2
    // 1. SENSOR_POLLING_TIMEOUT
    event.type = SENSOR_POLLING_TIMEOUT;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == IDLE);
    // 2. DRIVE_MSG_FB_RECEIVED
    event.type = DRIVE_MSG_FB_RECEIVED;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == WELCOME);
    // 3. SENSOR_POLLING_TIMEOUT
    event.type = SENSOR_POLLING_TIMEOUT;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == WELCOME);
    printf("TEST Case 2 Passed\n\n");
    
    // 4. DRIVE MESSAGE SEQUENCE
    event.type = DRIVE_MSG_LR_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // Depth
    event.type = DRIVE_MSG_DS_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // FB
    event.type = DRIVE_MSG_FB_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    printf("TEST Case 3 Passed\n\n");

    // 5. DRIVE MESSAGE OUT OF ORDER
    event.type = DRIVE_MSG_LR_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // FB
    event.type = DRIVE_MSG_FB_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // Depth
    event.type = DRIVE_MSG_DS_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    printf("TEST Case 4 Passed\n\n");

    // 5. DRIVE MESSAGE W POLL INTERJECTION
    event.type = DRIVE_MSG_LR_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // FB
    event.type = DRIVE_MSG_FB_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // SENSOR TIMEOUT
    event.type = SENSOR_POLLING_TIMEOUT;
    event.data = NULL;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // Depth
    event.type = DRIVE_MSG_DS_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    printf("TEST Case 5 Passed\n\n");

    //6. DRIVE WITH LAND INTERJECTION
    event.type = DRIVE_MSG_LR_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // FB
    event.type = DRIVE_MSG_FB_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == DRIVE);
    // LAND MESSAGE
    event.type = LAND_MSG_RECEIVED;
    event.data = NULL;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == LANDING);
    // Depth
    event.type = DRIVE_MSG_DS_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == LANDING);
    printf("TEST Case 6 Passed\n\n");

    //7. LAND status trigger
    subState.current_depth = WELCOME_DEPTH + DEPTH_TOLERANCE + 0.1;
    event.type = DRIVE_MSG_DS_RECEIVED;
    event.data = 10;
    insert_to_simple_queue(event);
    event_handler_sub();
    assert(subState.state == LANDING);
    event_handler_sub();
    printf("TEST Case 7 Passed\n\n");



    printf("ALL TESTS PASSED ✅");


}