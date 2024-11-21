#ifndef __INPUTS_REMOTE_H_
#define __INPUTS_REMOTE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

/* Function definitions */
int init_target_depth_knob(void);
uint16_t get_target_depth(void);
int analog2discrete(int val, int input_min, int input_max, int output_min, int output_max);

#ifdef __cplusplus
}
#endif

#endif /*__INPUTS_REMOTE_H */