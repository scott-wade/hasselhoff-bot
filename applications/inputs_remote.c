#include "inputs_remote.h"
#include "led_remote.h"
#include "stm32f4xx_mort2.h"
#include <cstdint>
#include <stdlib.h>
#include <stdio.h> 
#include "hardware_stm_adc.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_dma.h"


#define MAX_POT_VAL         3900 // Empirically measured max potentiometer value
#define TAR_DEP_DIG_0       2 // Index of first digit of target depth
#define TAR_DEP_DIG_1       3 // Index of second digit of target depth
#define JOYSTICK_TOLERANCE  10 // Max variation of joystick when at rest

// Variables to store DMA value outputs
uint16_t target_depth;
uint16_t joystick[2]; // [x,y]

/* 
 * Initialize the target depth knob with ADC and DMA
 */
int init_target_depth_knob(void) {
    // Init variable
    target_depth = 0;
    // Connected to 
    // Pin: PB0 
    // ADC: ADC12_IN8 (ADC 1 and 2 channel 8), use ADC1, channel 8
    // DMA: 2, channel 0 stream 0 works with ADC1
    initADCpinWithDMA(PORT_B, 
                      0,  /* pin_number */
                      ADC_1, /* adc_number */ 
                      8,  /* adc_channel*/
                      DMA_2,  /* dma_number */
                      0,  /* dma_channel */ 
                      0,  /* dma_stream */ 
                      &target_depth /* dest_addr */);

    return 0; // success
}

/*
 * Returns the target depth that is measured from the potentiometer as a discrete value
 */
uint16_t get_target_depth(void) {
    return analog2discrete(target_depth, 
                           0, MAX_POT_VAL, // Input range
                           1, 17); // Desired range
}

void read_target_depth (void) {
    static uint16_t prev_val = 0;
    uint16_t curr_val = get_target_depth();

    if (prev_val != curr_val) {
        // If value changed, set the led display value
        int first_dig, second_dig;
        first_dig = curr_val / 10; // Integer division
        second_dig = curr_val % 10; // Remainder

        // Set the values on the led display
        set_led_disp_val(TAR_DEP_DIG_0, first_dig);
        set_led_disp_val(TAR_DEP_DIG_1, second_dig);
    }
    prev_val = curr_val; // Set current to old
}

/*
 * Initialize the joystick analog inputs
 */
int init_joysticks(void) {
    // Init variable 
    joystick[0] = 0;
    joystick[1] = 0;
    // Y-stick connected to 
    // Pin: PA4
    // ADC: ADC12_IN4 (ADC 1 and 2 channel 4), use ADC2, channel 4
    // DMA: 2, channel 0 works with ADC1
    // ---
    // X-stick connected to 
    // Pin: PA5
    // ADC: ADC12_IN5 (ADC 1 and 2 channel 5), use ADC2, channel 5
    // DMA: 2, channel 1 stream 2 works with ADC2
    int port_numbers[] = {PORT_A, PORT_A};
    int pin_numbers[] = {5, 4};
    uint32_t adc_channels[] = {5, 4};
    initADCpinsWithDMA(port_numbers, 
                       pin_numbers,
                       // ADC
                       ADC_2, /* adc_number */ 
                       adc_channels,
                       // DMA
                       DMA_2,  /* dma_number */
                       1,  /* dma_channel */ 
                       2,  /* dma_stream */ 
                       joystick, /* dest_addr */
                       2 /* num_pins */ );

    return 0; // success
}

/*
 * Returns joystick x value
 */
uint16_t get_joystick_x (void) {
    return joystick[0];
}
/*
 * Returns joystick y value
 */
uint16_t get_joystick_y (void) {
    return joystick[1];
}

/*
 * Task to keep reading the joystick values
 */
void read_joysticks (void) {
    static uint16_t prev_joy_x = 0;
    static uint16_t prev_joy_y = 0;
    // Read joystick values
    uint16_t joy_x = get_joystick_x();
    uint16_t joy_y = get_joystick_y();

    // Check if values changed!
    int diff_x = abs(joy_x - prev_joy_x);
    if (diff_x > JOYSTICK_TOLERANCE) {
        // printf("JOY X CHANGED: %d vs. %d = %d\n", joy_x, prev_joy_x,diff_x );
    }
    int diff_y = abs(prev_joy_y - joy_y);
    if (diff_y > JOYSTICK_TOLERANCE) {
        // printf("JOY Y CHANGED: %d vs. %d = %d\n", joy_y, prev_joy_y, diff_y);
    }
    // Set previous values
    prev_joy_x = joy_x;
    prev_joy_y = joy_y;
}


/* *******************************************************************************
                    INPUTS UTILITY FUNCTIONS
   ******************************************************************************* */
/*
 * Convert an analog integer that is in the [input_min, input_max] range
 * and maps to the desired [output_min, output_max] range
 * Then discretize it so that it only returns an integer
 * @param val: the analog value to convert
 * @param input_min: the min value that the val can take
 * @param input_max: the max value that the val can take
 * @param output_min: min value of the desired range
 * @param output_max: max value of the desired range
 */
int analog2discrete(int val, int input_min, int input_max, int output_min, int output_max) {
    // Ensure the input is within the valid range
    if (val < input_min) {
        val = input_min;
    } else if (val > input_max) {
        val = input_max;
    }

    // Map val from input range [input_min, input_max] to output range [output_min, output_max]
    return (int)((float)(val - input_min) / (input_max - input_min) * (output_max - output_min) + output_min);
}


