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
  #define BALLAST_CW_PROP_GND (int[]){3,3} // GPIO D3
  #define BALLAST_CCW_PROP_PIN (int[]){3,13} // GPIO D13
  #define BALLAST_CCW_PROP_GND (int[]){-1,-1} // Counterpart of D13 is already tied to GND
  #define PROPULSION_CW_PROP_PIN (int[]){3,14} // GPIO D14
  #define PROPULSION_CW_PROP_GND (int[]){1,3) // GPIO B3
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

