//
// Created by Nathan Zimmerer on November 2024.
//

#ifndef __SUB_CLOCK_H_
#define __SUB_CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Macros for Everyone--------------------------------------------------------*/

/*Function definitions---------------------------------------------------------*/
void initSubClock(void);
void wrapTimer(void);
double getSubMS(void);
void sub_sleep(float duration);

#ifdef __cplusplus
}
#endif
#endif // SUB_TIMER_H
