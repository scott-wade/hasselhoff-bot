#ifndef __INPUTS_REMOTE_H_
#define __INPUTS_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern uint8_t land_button_pressed = 0;


/* Function definitions */
int init_target_depth_knob(void);
uint16_t get_target_depth();
int analog2discrete(int val, int input_min, int input_max, int output_min, int output_max);
int init_joysticks(void);
uint16_t get_joystick_x(int prev_val);
uint16_t get_joystick_y(int prev_val);
uint8_t read_target_depth (void);
uint16_t[] read_joysticks (void);


#ifdef __cplusplus
}
#endif

#endif /*__INPUTS_REMOTE_H */