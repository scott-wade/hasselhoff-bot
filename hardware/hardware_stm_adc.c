#include "hardware_stm_adc.h"
#include "hardware_stm_dma.h"
#include "hardware_stm_gpio.h"
#include "stm32f4xx_rcc_mort.h"
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>


/*
 * Initialize ADC with DMA
 * @param adc_num: 1-3 number for ADC 1, 2, 3
 * @param adc_channel: 0-15, selected the channel on the ADC to use
 */
void initADCwithDMA(int adc_num, uint32_t adc_channel) {
    // Set addresses
    uint32_t adc_base_address = mapAdcNumbertoBaseAddress(adc_num);
    uint32_t * sr_register = (uint32_t *)(long)(adc_base_address + ADC_SR_REGISTER_OFFSET);
    uint32_t * cr1_register = (uint32_t *)(long)(adc_base_address + ADC_CR1_REGISTER_OFFSET);
    uint32_t * cr2_register = (uint32_t *)(long)(adc_base_address + ADC_CR2_REGISTER_OFFSET);
    uint32_t * smpr1_register = (uint32_t *)(long)(adc_base_address + ADC_SMPR1_REGISTER_OFFSET);
    uint32_t * smpr2_register = (uint32_t *)(long)(adc_base_address + ADC_SMPR2_REGISTER_OFFSET);
    uint32_t * sqr1_register = (uint32_t *)(long)(adc_base_address + ADC_SQR1_REGISTER_OFFSET);
    uint32_t * sqr2_register = (uint32_t *)(long)(adc_base_address + ADC_SQR2_REGISTER_OFFSET);
    uint32_t * sqr3_register = (uint32_t *)(long)(adc_base_address + ADC_SQR3_REGISTER_OFFSET);
    uint32_t * dr_register = (uint32_t *)(long)(adc_base_address + ADC_DR_REGISTER_OFFSET);

    uint32_t* reg_pointer;
    // 1. Enable the APB2 clock
    enableAHB2ADCclock(adc_num);
    // 2. Setup the clock Prescaler to divide the clock by 4 by writing to
    //      the ADC_COMMON_CCR_REGISTER
    reg_pointer = (uint32_t*) ADC_COMMON_CCR_REGISTER;
    *reg_pointer = ADC_PRESCALER_4;
    // Clear status register
    reg_pointer = (uint32_t*) sr_register;
    *reg_pointer = 0;
    // 3. Configure the ADC to be 12 bit resolution, and end of conversion interrupt 
    //      disabled by writing to the CR1 register in ADC
    reg_pointer = (uint32_t*) cr1_register;
    *reg_pointer = 0;
    // 4. Configure the ADC to have the external trigger disabled, right data alignment, 
    //      ADC_DDS, DMA, EOC set at the end of each regular conversion and single conversion 
    //      enabled by setting EOCS to the CR2 register in ADC
    reg_pointer = (uint32_t*) cr2_register;
    *reg_pointer = ADC_EOCS + ADC_DDS + ADC_DMA;
    // 5. Select to have 1 conversion by writing a 0 to the L bits in SQR1 register in ADC
    reg_pointer = (uint32_t*) sqr1_register;
    *reg_pointer = ADC_1_CONVERSIONS;
    // 6. Configure the sequence of conversions by writing channel number to SQ1 in the SQR3 register of ADC
    reg_pointer = (uint32_t*) sqr3_register;
    *reg_pointer = (adc_channel << ADC_SQ1);
    // 7. Choose 480 cycles of sampling for channel number in ADC by writing all ones to the SMP7 bits in the ADC SMPR2 register.
    reg_pointer = (uint32_t*)smpr2_register;
    *reg_pointer = (uint32_t)(0b111<<(adc_channel*3));
    // 8. Turn the ADC on by setting the ADON bit in the CR2 register in ADC
    reg_pointer = (uint32_t*) cr2_register;
    *reg_pointer = *reg_pointer | ADC_ADON;
    // 11. NOTE: A conversion will not start until the SWSTART bit in CR2 bit is set, but we need to wait a few clock cycles for that.
}

/*
 * Initialize a pin with ADC and DMA
 * @param port_number: pin's port number
 * @param pin_number: pin number
 * @param adc_number: number of the ADC to use
 * @param dma_number: select either DMA1 or DMA2 (1-2)
 * @param dma_channel: the dma channel (0-7) that corresponds to the ADC, eg. DMA 2 stream 0 channel 2 for ADC3
 * @param dest_addr: destination address to store the value into
 */
void initADCpinWithDMA(int port_number, int pin_number, int adc_number, uint32_t adc_channel,
                       int dma_number, int dma_channel, uint16_t* dest_addr) {
    int num_transfers = 1; // Only 1 transfer in this case
    initGPIOasAnalog(port_number, pin_number); // Init analog pin
    initADCwithDMA(adc_number, adc_channel); // Init ADC
    initDMAForAdc(dma_number, dma_channel, adc_number, num_transfers, dest_addr); // Init DMA
}


/*
 * Get ADC value directly from the ADC register
 * @param adc_num, what ADC to use (1-2)
 */
uint32_t get_adc_val(int adc_num) {
    uint32_t* reg_pointer;
    uint32_t value;
    uint32_t adc_base_address = mapAdcNumbertoBaseAddress(adc_num);
    uint32_t * sr_register = (uint32_t *)(long)(adc_base_address + ADC_SR_REGISTER_OFFSET);
    uint32_t * dr_register = (uint32_t *)(long)(adc_base_address + ADC_DR_REGISTER_OFFSET);

    // Read value of status register
    value = *reg_pointer;

    // Check if end of conversion (EOC)
    if ((*sr_register & ADC_EOC) > 0) {
        // Only first 12 bits are relevant
        value = *dr_register & 0xFFF;
        return value;
    } else {
        fprintf(stderr, "ADC_EOC not set\n");
    }
}

/*
 * Start the conversion for specified ADC
 * @param adc_num, what ADC to use (1-2)
 */
void startADCConversion(int adc_num) 
{
    // Need to have a small delay before enabling SWSTART
    // to allow ADC time to actually turn on
    uint32_t adc_base_address = mapAdcNumbertoBaseAddress(adc_num);
    uint32_t * sr_register = (uint32_t *)(long)(adc_base_address + ADC_SR_REGISTER_OFFSET);
    uint32_t * cr2_register = (uint32_t *)(long)(adc_base_address + ADC_CR2_REGISTER_OFFSET);
    // Clear status register
    *sr_register = 0;
    // Set the SWSTART bit in CR2 register
    *cr2_register = *cr2_register | ADC_SWSTART;
}

/* *******************************************************************************
                    ADC UTILITY FUNCTIONS
   ******************************************************************************* */
uint32_t mapAdcNumbertoBaseAddress(int adc_num)
{
    uint32_t adc_base_address;
    switch (adc_num) {
        case 1 : {adc_base_address = ADC_1_BASE_ADDRESS; break;}
        case 2 : {adc_base_address = ADC_2_BASE_ADDRESS; break;}
        case 3 : {adc_base_address = ADC_3_BASE_ADDRESS; break;}
        default : fprintf(stderr, "Received Unknown Adc Number %d at Base Address Map\n", adc_num);
    }

    return adc_base_address;
}
void enableAHB2ADCclock(int adc_num)
{
    switch (adc_num) {
        case 1 : {RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); break;}
        case 2 : {RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); break;}
        case 3 : {RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); break;}
        default : fprintf(stderr, "Received Unknown Port Number at AHB2 clock enable\n");
    }

}
