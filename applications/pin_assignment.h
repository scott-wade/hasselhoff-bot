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
  #define BALLAST_CW_PROP_PIN (int[]){3,12} // GPIO D12
  #define BALLAST_CW_PROP_GND (int[]){4,7} // Counterpart of D12 is already tied to GND (setting same pin as D13 counterpart)
  #define BALLAST_CCW_PROP_PIN (int[]){3,13} // GPIO D13
  #define BALLAST_CCW_PROP_GND (int[]){4,7} // GPIO E7
  #define PROPULSION_CW_PROP_PIN (int[]){3,14} // GPIO D14
  #define PROPULSION_CW_PROP_GND (int[]){4,7} // GPIO E7 counterpart of D14 already tied to ground
  #define PROPULSION_CCW_PROP_PIN (int[]){3,15} // GPIO D15
  #define PROPULSION_CCW_PROP_GND (int[]){0,4} // GPIO A4
  
  // Timer and Channel Assignments
  #define MOTOR_TIMER 4
  #define BALLAST_CW_PROP_CH 1
  #define BALLAST_CCW_PROP_CH 2
  #define PROPULSION_CW_PROP_CH 3
  #define PROPULSION_CCW_PROP_CH 4

  /* Infrared Sensor Pin Assignments */
  #define IR_RANGE_PORT_NUM 5
  #define IR_RANGE_PIN_NUM 0

