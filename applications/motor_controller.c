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
  #include "sub_clock.h"

  /* MOTOR PROPERTIES */
  #define IDLE_DUTY_CYCLE 0.75
  #define FWD_DEADZONE 0.03
  #define REV_DEADZONE 0.03
  #define REV_MIN 0.5

  /* MOTOR CONFIGURATION */
  // Left Propulsion Prop -> CW PROPULSION PROP
  // Right Propulsion Prop -> CCW PROPULSION PROP
  // Front Ballast Prop -> CW BALLAST PROP
  // Rear Ballast Prop -> CCW BALLAST PROP
  
  /* 
    Initialize Motor Hardware and Run Propulsion Self test  
  */
  void initMotorHardware()
  {
      initMotorGPIOs();
      initMotorTimers();
      runPropulsionSelfTest();
  }

  /* 
    Initialize Motor GPIOs to relay PWM signals 
  */
  void initMotorGPIOs()
  {
      /* Initialize Each Prop Pin GPIO into ALT FN 2 */
      initGPIOasMode(BALLAST_CW_PROP_PIN[0], BALLAST_CW_PROP_PIN[1], 2, 0, 0, 0, 2);
      initGPIOasMode(BALLAST_CCW_PROP_PIN[0], BALLAST_CCW_PROP_PIN[1], 2, 0, 0, 0, 2);
      initGPIOasMode(PROPULSION_CW_PROP_PIN[0], PROPULSION_CW_PROP_PIN[1], 2, 0, 0, 0, 2);
      initGPIOasMode(PROPULSION_CCW_PROP_PIN[0], PROPULSION_CCW_PROP_PIN[1], 2, 0, 0, 0, 2);

  }
  
  /* 
    Initialize Motor Timers and PWM on each channel 
  */
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
      initPWMonChannel(MOTOR_TIMER, BALLAST_CW_PROP_CH, IDLE_DUTY_CYCLE);
      initPWMonChannel(MOTOR_TIMER, BALLAST_CCW_PROP_CH, IDLE_DUTY_CYCLE);
      initPWMonChannel(MOTOR_TIMER, PROPULSION_CW_PROP_CH, IDLE_DUTY_CYCLE);
      initPWMonChannel(MOTOR_TIMER, PROPULSION_CCW_PROP_CH, IDLE_DUTY_CYCLE);

      enableTimer(MOTOR_TIMER);

      
  }

  /* 
    Run Propulsion self test
    Set 75% duty cycle on all motors and sleep for a second 
  */
  void runPropulsionSelfTest()
  {
      
      setDutyCycle(MOTOR_TIMER, BALLAST_CW_PROP_CH, IDLE_DUTY_CYCLE);
      setDutyCycle(MOTOR_TIMER, BALLAST_CCW_PROP_CH, IDLE_DUTY_CYCLE);
      setDutyCycle(MOTOR_TIMER, PROPULSION_CW_PROP_CH, IDLE_DUTY_CYCLE);
      setDutyCycle(MOTOR_TIMER, PROPULSION_CCW_PROP_CH, IDLE_DUTY_CYCLE);
      
      sub_sleep(1.0);
  }

  /* 
    Converts joystick inputs into PWM signals to the CW and CCW motors 
  */
  void propulsionControl(float x_input, float y_input)
  {
    float idle_thrust;
    float left_motor_duty;
    float right_motor_duty;
    if (x_input >= 0) {
        idle_thrust = IDLE_DUTY_CYCLE + FWD_DEADZONE;
        float thrust = idle_thrust + 0.5*(1-idle_thrust)*x_input;
        float steering_modifier = 0.5*(1-idle_thrust)*y_input;

        left_motor_duty = thrust + steering_modifier;
        right_motor_duty = thrust - steering_modifier;
    }
    else {
        idle_thrust = IDLE_DUTY_CYCLE - REV_DEADZONE;
        float thrust = idle_thrust - 0.5*(idle_thrust-REV_MIN)*x_input;
        float steering_modifier = 0.5*(idle_thrust-REV_MIN)*y_input;

        left_motor_duty = thrust + steering_modifier;
        right_motor_duty = thrust - steering_modifier;
    }
    
    if (left_motor_duty > 1) {left_motor_duty = 1.0;}
    else if (left_motor_duty < idle_thrust) { left_motor_duty = idle_thrust;}

    if (right_motor_duty > 1) {right_motor_duty = 1.0;}
    else if (right_motor_duty < idle_thrust) { right_motor_duty = idle_thrust;}

    setDutyCycle(MOTOR_TIMER, PROPULSION_CW_PROP_CH, left_motor_duty);
    setDutyCycle(MOTOR_TIMER, PROPULSION_CCW_PROP_CH, right_motor_duty);
  }

  /* 
    Navigate to a target depth by setting PWM signals 
  */
  void ballastControl(float target_depth)
  {
     float current_depth;
     //current_depth = readDepthSensor();

     if (current_depth > target_depth) { // Raise Depth
        
        float idle_duty_cycle = IDLE_DUTY_CYCLE + FWD_DEADZONE; 
        float thrust = idle_duty_cycle + 0.5*(1-idle_duty_cycle);

        setDutyCycle(MOTOR_TIMER, BALLAST_CW_PROP_CH, thrust);
        setDutyCycle(MOTOR_TIMER, BALLAST_CCW_PROP_CH, thrust);
     }
  }