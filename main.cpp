#include <cstdint>
#include "main.h"
#include "debug_mort.h"
#include "state_machine/state_machine_sub.h"
#include "state_machine/state_machine_remote.h"
#include "tests/test_comms.h"

#define WHICH_NUCLEO 2 //change for compilation, 0 for remote, 1 for submarine, 2 for debug

int main(void){
    test_comms();
    return 1;
}
