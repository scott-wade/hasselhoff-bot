/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_mort2.h"
#include <cstdio>
#include <cstdlib>
#include "debug_mort.h"
#include "hardware_stm_gpio.h"
// #include "mbed.h"
#include <cstdio>
#include <cstdlib>

/* MACROS for everyone--------------------------------------------------------*/

#define bool uint8_t

/*Function definitions---------------------------------------------------------*/
void delay(uint16_t ms);


#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */
