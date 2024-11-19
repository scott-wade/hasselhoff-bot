/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_GPIO_H_
#define __HARDWARE_STM_GPIO_H_

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

/*
 * mapping port number: 0->A, 1->B, ... 7->H
 * mode: 0-input, 1-output, 2-AF
 * open_drain: 0-pupd, 1-open drain
 * pupd: 0-floating, 1-pull up, 2-pull down
 * init_output: [0,1] initial value if in output mode 
 * alt_func: number of alternate function mode
**/
#define MODE_IN         0
#define MODE_OUT        1
#define MODE_AF2        2
#define OD_PUPD         0
#define OD_OPEN_DRAIN   0
#define PUPD_FLOAT      0
#define PUPD_UP         1
#define PUPD_DOWN       2



/*Function definitions---------------------------------------------------------*/
// GPIO initialization
void initGPIOasMode(uint8_t port_number, uint8_t pin_number, uint8_t mode, uint8_t open_drain, uint8_t pupd, uint8_t init_output, uint8_t alt_func);

// SET, CLEAR and READ functions
void SETorCLEARGPIOoutput(int port_number, int pin_number, int set);
void SETorCLEARGPIOpins(int port_numbers[], int pin_numbers[], int set_flags[], int num_pins);
int readGPIOinput(int port_number, int pin_number);
int readGPIOoutput(int port_number, int pin_number);

// UTILITIES
uint32_t mapPortNumbertoBaseAddress(int port_number);
void enableAHB1RCCclock(int port_number);


#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */
