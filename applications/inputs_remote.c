#include "inputs_remote.h"
#include "stm32f4xx_mort2.h"
#include <stdlib.h>
#include <stdio.h> 
#include "hardware_stm_adc.h"
#include "hardware_stm_gpio.h"
#include "hardware_stm_dma.h"

#define MAX_POT_VAL     3900 // Empirically measured max potentiometer value


// Variables to store DMA value outputs
uint16_t target_depth;

/* 
 * Initialize the target depth knob with ADC and DMA
 */
int init_target_depth_knob(void) {
    // Connected to 
    // Pin: PB0 
    // ADC: ADC12_IN8 (ADC 1 and 2 channel 8), use ADC1, channel 8
    // DMA: 2, channel 0 works with ADC1
    initADCpinWithDMA(PORT_B, 
                      0,  /* pin_number */
                      ADC_1, /* adc_number */ 
                      8,  /* adc_channel*/
                      DMA_2,  /* dma_number */
                      0,  /* dma_channel */ 
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
