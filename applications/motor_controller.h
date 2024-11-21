#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#ifdef __cplusplus
 extern "C" {
#endif

void initMotorHardware();
void initMotorGPIOs();
void initMotorTimers();
void runPropulsionSelfTest(); 
void propulsionControl(float x_input, float y_input);
void ballastControl(float target_depth);

#ifdef __cplusplus
}
#endif

#endif /*__MOTOR_CONTROLLER_H */