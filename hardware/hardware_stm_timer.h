#ifndef __HARDWARE_STM_TIMER_H_
#define __HARDWARE_STM_TIMER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_mort2.h"


/*Function definitions---------------------------------------------------------*/
void initTimer (int timer_number, uint16_t psc, uint16_t arr);
void enableTimer(int timer_number);
void initPWMonChannel(int timer_number, int channel_number, float duty_cycle);

uint32_t * mapTimerNumberToBaseAddress(int timer_number);
void enableAPB1RCCclock(int timer_number);

#ifdef __cplusplus
}
#endif

#endif /*__TIMER_H */