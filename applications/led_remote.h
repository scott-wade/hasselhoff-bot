#ifndef __LED_REMOTE_H_
#define __LED_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

/* Function definitions */
int set_digit_value(uint8_t val);
int select_digit(uint8_t selected_digit);
int set_seg_led(uint8_t digit, uint8_t val);
int init_status_leds(void);
int set_blue_led(void);
int clear_blue_led(void);
int set_yellow_led(void);
int clear_yellow_led(void);
int set_green_led(void);
int clear_green_led(void);
int set_white_led(void);
int clear_white_led(void);
int set_rgb_green_led(void);
int clear_rgb_green_led(void);
int set_rgb_red_led(void);
int clear_rgb_red_led(void);

#ifdef __cplusplus
}
#endif

#endif /*__LED_REMOTE_H */