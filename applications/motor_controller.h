#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#ifdef __cplusplus
 extern "C" {
#endif

void initMotorHardware(void);
void initMotorGPIOs(void);
void initMotorTimers(void);
void runPropulsionSelfTest(void); 
void propulsionControl(float x_input, float y_input);
void ballastControl(float target_depth);

#ifdef __cplusplus
}
#endif

#endif /*__MOTOR_CONTROLLER_H */