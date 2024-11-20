/**
  ******************************************************************************
  * @file    sub_clock.c
  * @author  nathanz@andrew.cmu.edu
  * @version 1.0
  * @date    November-2024
  * @brief   Manages the submarine running clock
  ******************************************************************************
  */

 #include "applications/sub_clock.h"
 #include "hardware/hardware_stm_timer.h"

 /*******************************
* Macros
*******************************/
#define CLOCK_TIMER 3 // ie which timer is being used to time things

 /*******************************
* Declarations
*******************************/
static uint16_t timeWrapCount;

/*******************************
* Functions
*******************************/
// initialize Timer3 and additional var to serve as a MSB counter for Timer3 wraps
void initSubClock(void)
{
    timeWrapCount = 0;
    // initialize timer3 in upcounting mode w/ ticks at 10 kHz (ie 0.1 ms res)
    // arr calibrated for timer to wrap at 2 Hz
    initTimer(CLOCK_TIMER, 8999, 5000);
    // enable to timer to interrupt for when it wraps
    initTimerIntOverflow(CLOCK_TIMER);
    // enable the timer
    enableTimer(CLOCK_TIMER);
}

// function iterates the counter intended for timer overflow
// TODO:
 // add a call to this from the interrupt controller
void wrapTimer(void)
{
    timeWrapCount++;
    printf("debug time wrap");
}

// calculate & return the number of ms surpassed
double getSubMS(void)
{
    double msPassed;
    msPassed = 500 * timeWrapCount + 0.1 * getTIMCNT(CLOCK_TIMER);
    return msPassed;
}