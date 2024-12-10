#ifndef __INPUTS_REMOTE_H_
#define __INPUTS_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define MAX_POT_VAL             4095 // Empirically measured max potentiometer value
#define TAR_DEP_DIG_0           2 // Index of first digit of target depth
#define TAR_DEP_DIG_1           3 // Index of second digit of target depth
#define MAX_JOY_VAL             4094 // Empirically measured max joystick value

extern uint8_t land_button_pressed;


/* Function definitions */
int init_target_depth_knob(void);
uint16_t get_target_depth();
int analog2discrete(int val, int input_min, int input_max, int output_min, int output_max);
int init_joysticks(void);
uint16_t get_joystick_x(int prev_val);
uint16_t get_joystick_y(int prev_val);
uint8_t read_target_depth (void);
void read_joysticks (uint16_t*);


#ifdef __cplusplus
}
#endif

#endif /*__INPUTS_REMOTE_H */