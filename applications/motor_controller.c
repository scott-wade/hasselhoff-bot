/**
  ******************************************************************************
  * @file    motor_controller.c 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    November 2024
  * @brief   Controls PWM input to motors
  ******************************************************************************
  */

  #include "motor_controller.h"
  #include "pin_assignment.h"
  #include "../hardware/hardware_stm_gpio.h"
  #include "../hardware/hardware_stm_timer.h"

  void initMotorHardware()
  {
      initMotorGPIOs();
      initMotorTimers();
      enableMotorTimer();
  }

  void initMotorGPIOs()
  {
      /* Initialize Each Prop Pin GPIO into ALT FN 2 */
      initGPIOasMode(BALLAST_CW_PROP_PIN[0], BALLAST_CW_PROP_PIN[1], 2, 
        0, 0, 0, 2);
      initGPIOasMode(BALLAST_CCW_PROP_PIN[0], BALLAST_CCW_PROP_PIN[1], 2, 
        0, 0, 0, 2);
      initGPIOasMode(PROPULSION_CW_PROP_PIN[0], PROPULSION_CW_PROP_PIN[1], 2, 
        0, 0, 0, 2);
      initGPIOasMode(PROPULSION_CCW_PROP_PIN[0], PROPULSION_CCW_PROP_PIN[1], 2, 
        0, 0, 0, 2);

  }

  void initMotorTimers()
  {
      // All APB1 clocks run at 90MHz
      // ApisQueen wants the PWM signal period to be 2ms i.e., 500Hz
      uint16_t desired_timer_freq = 500;
      uint16_t arr = 60000;
      uint16_t psc = 90000000/(arr*desired_timer_freq) - 1; // psc = 2
      
      // Initialize Timer 4
      initTimer(MOTOR_TIMER, psc, arr);

      // Initialize All four channels of timer 4
      float idle_duty_cycle = 0.8; // TODO : change if necessary
      initPWMonChannel(MOTOR_TIMER, BALLAST_CW_PROP_CH, idle_duty_cycle);
      initPWMonChannel(MOTOR_TIMER, BALLAST_CCW_PROP_CH, idle_duty_cycle);
      initPWMonChannel(MOTOR_TIMER, PROPULSION_CW_PROP_CH, idle_duty_cycle);
      initPWMonChannel(MOTOR_TIMER, PROPULSION_CCW_PROP_CH, idle_duty_cycle);
  }

  void enableMotorTimer()
  {
      enableTimer(MOTOR_TIMER);
  }


 // TODO : joystick inputs to motor PWM conversion