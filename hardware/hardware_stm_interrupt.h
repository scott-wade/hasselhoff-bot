/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_INTERRUPT_H_
#define __HARDWARE_STM_INTERRUPT_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_mort2.h"


/* Macros for Everyone--------------------------------------------------------*/
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7
#define PIN_8   8
#define PIN_9   9
#define PIN_10  10



/*Function definitions---------------------------------------------------------*/
// Interrupt initialization
void enableSPI1Interrupt(void);
void enableSPI4Interrupt(void);
void enableNVIC_StdTimer(int timer_number);
void enableEXTI6OnPortC(void);


#ifdef __cplusplus
}
#endif

#endif /*__INTERRUPT_H */
