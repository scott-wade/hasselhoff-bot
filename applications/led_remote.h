#ifndef __LED_REMOTE_H_
#define __LED_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"



/* Function definitions */
int set_digit_value(int val);
int select_digit(uint8_t selected_digit);
int set_seg_led(uint8_t digit, int val);
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
int init_seg_display( void );
void cycle_led_display( void );
int set_led_disp_vals(int values[4]);
int set_led_disp_val(int digit, int value);
void welcome_remote (void);
int countdown_timer (void);
void read_sub_status(void);


#ifdef __cplusplus
}
#endif

#endif /*__LED_REMOTE_H */