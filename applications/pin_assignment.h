/**
  ******************************************************************************
  * @file    pin_assignment.h 
  * @author  kkaruman@andrew.cmu.edu
  * @version 1.0
  * @date    November 2024
  * @brief   Controls STM32F446ze Pin Assignment for Hasselhoff bot
  ******************************************************************************
  */

  /* MOTOR PWM PIN ASSIGNMENTS */
  #define BALLAST_CW_PROP_PIN (int[]){1,6} // GPIO B6
  #define BALLAST_CCW_PROP_PIN (int[]){1,7} // GPIO B7
  #define PROPULSION_CW_PROP_PIN (int[]){1,8} // GPIO B8
  #define PROPULSION_CCW_PROP_PIN (int[]){1,9} // GPIO B9
  
  // Timer and Channel Assignments
  #define MOTOR_TIMER 4
  #define BALLAST_CW_PROP_CH 1
  #define BALLAST_CCW_PROP_CH 2
  #define PROPULSION_CW_PROP_CH 3
  #define PROPULSION_CCW_PROP_CH 4

  /* Infrared Sensor Pin Assignments */
  #define IR_RANGE_PORT_NUM 5
  #define IR_RANGE_PIN_NUM 0

