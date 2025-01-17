#include "inputs_remote.h"
#include "led_remote.h"
#include "state_machine_sub.h"
#include "stm32f4xx_mort2.h"
#include <cstdint>
#include <stdlib.h>
#include <stdio.h> 
#include "hardware_stm_adc.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_dma.h"
#include "state_machine_remote.h"
#include "state_machine_SPI.h"
#include "hardware_stm_interrupt.h"


// Variables to store DMA value outputs
uint16_t target_depth;
uint16_t joystick[2]; // [x,y]
uint8_t land_button_pressed = 0;


/**
 * Filter new analog value using a combination of both previous and current values
 * @param prev_val is the previous measured value
 * @param new_val is the current measured value
 * @param alpha is 0-1 value that is percent of new value to incorporate
 */
int analog_filter(int prev_val, int new_val, float alpha) {
    if (alpha < 0 || alpha > 1) {
        fprintf(stderr, "[ERRPR] alpha must be between 0 and 1\n");
        return new_val;
    }
    return (int) (alpha * new_val + (1 - alpha) * prev_val);
}

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
    // Filter the value
    // Map to desired range
    uint16_t processed_val = analog2discrete(target_depth, 
                        0, MAX_POT_VAL, // Input range
                        1, 17); // Desired range
    
    return processed_val;
}

uint8_t read_target_depth (void) {
    uint8_t curr_val = get_target_depth();

    // Update LED display with depth value
    int first_dig = curr_val / 10; // Integer division
    int second_dig = curr_val % 10; // Remainder

    // Set the values on the led display
    set_led_disp_val(TAR_DEP_DIG_0, first_dig);
    set_led_disp_val(TAR_DEP_DIG_1, second_dig);

    return curr_val;
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
    
    enableEXTI6OnPortC();

    return 0; // success
}

/*
 * Returns joystick x value
 */
uint16_t get_joystick_x (int prev_val) {
    uint16_t processed_val = analog_filter(prev_val, joystick[0], 0.7 /* alpha */);
    return processed_val;
}
/*
 * Returns joystick y value
 */
uint16_t get_joystick_y (int prev_val) {
    uint16_t processed_val = analog_filter(prev_val, joystick[1], 0.7 /* alpha */);
    return processed_val;
}

/*
 * Task to keep reading the joystick values
 */
void read_joysticks (uint16_t* joy_inputs) {
    static uint16_t prev_joy_x = 0;
    static uint16_t prev_joy_y = 0;
    // Read joystick values
    uint16_t joy_x = get_joystick_x(prev_joy_x);
    uint16_t joy_y = get_joystick_y(prev_joy_y);

    // Map to 0-10
    joy_x = analog2discrete(joy_x, 
                            0, MAX_JOY_VAL, // Input range
                            0, 10); // Desired range
    joy_y = analog2discrete(joy_y, 
                            0, MAX_JOY_VAL, // Input range
                            0, 10); // Desired range


    // Set previous values
    prev_joy_x = joy_x;
    prev_joy_y = joy_y;

    joy_inputs[0] = joy_x;
    joy_inputs[1] = joy_y;

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


