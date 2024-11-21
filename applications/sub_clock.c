/**
  ******************************************************************************
  * @file    sub_clock.c
  * @author  nathanz@andrew.cmu.edu
  * @version 1.0
  * @date    November-2024
  * @brief   Manages the submarine running clock
  ******************************************************************************
  */

 #include "sub_clock.h"
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
void wrapTimer(void)
{
    timeWrapCount++;
    // printf("ms transpired %.1f \n", getSubMS()); // debug
}

// calculate & return the number of ms surpassed
double getSubMS(void)
{
    double msPassed;
    msPassed = 500 * timeWrapCount + 0.1 * getTIMCNT(CLOCK_TIMER);
    return msPassed;
}

void sub_sleep(float duration)
{
    float elapsed_duration = 0.0;
    float sleep_cycle_start_time = getSubMS();

    while (elapsed_duration <= 1000*duration){
        elapsed_duration += (getSubMS() - sleep_cycle_start_time);
    }

    return;
}