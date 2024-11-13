#ifndef __HARDWARE_STM_TIMER_H_
#define __HARDWARE_STM_TIMER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_mort2.h"


/*Function definitions---------------------------------------------------------*/
void initTim3 (uint16_t psc, uint16_t arr);
uint16_t getTIM3CNT(void);
void resetTIM3(void);

#ifdef __cplusplus
}
#endif

#endif /*__TIMER_H */