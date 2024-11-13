#ifndef __MOTOR_CONTROLLER_H_
#define __MOTOR_CONTROLLER_H_

#ifdef __cplusplus
 extern "C" {
#endif

void initMotorGPIOs();
void initMotorTimers();
void enableMotorTimer();

#ifdef __cplusplus
}
#endif

#endif /*__MOTOR_CONTROLLER_H */