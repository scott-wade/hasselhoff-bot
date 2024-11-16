#include "hardware_stm_adc.h"
#include "hardware_stm_dma.h"
#include "stm32f4xx_rcc_mort.h"


// void initADC3_5_withDMA( void ) {
//     uint32_t* reg_pointer;
//     // 1. Enable the APB2 clock:
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
//     // Init GPIO F7 as analog
//     initGpioF7AsAnalog();
//     // Init and enable ADC3
//     initDMAForAdc3_1channel();
//     enableDMAForAdc3_1channel();
//     // 2. Setup the clock Prescaler to divide the clock by 4 by writing to
//     //      the ADC_COMMON_CCR_REGISTER
//     reg_pointer = (uint32_t*) ADC_COMMON_CCR_REGISTER;
//     *reg_pointer = ADC_PRESCALER_4;
//     // Clear status register
//     reg_pointer = (uint32_t*) ADC_3_SR_REGISTER;
//     *reg_pointer = 0;
//     // 3. Configure the ADC to be 12 bit resolution, and end of conversion interrupt 
//     //      disabled by writing to the CR1 register in ADC 3
//     reg_pointer = (uint32_t*) ADC_3_CR1_REGISTER;
//     *reg_pointer = 0;
//     // 4. Configure the ADC to have the external trigger disabled, right data alignment, 
//     //      ADC_DDS, DMA, EOC set at the end of each regular conversion and single conversion 
//     //      enabled by setting EOCS to the CR2 register in ADC3.
//     reg_pointer = (uint32_t*) ADC_3_CR2_REGISTER;
//     *reg_pointer = ADC_EOCS + ADC_DDS + ADC_DMA;
//     // 5. Select to have 1 conversion by writing a 0 to the L bits in SQR1 register in ADC3
//     reg_pointer = (uint32_t*) ADC_3_SQR1_REGISTER;
//     *reg_pointer = ADC_1_CONVERSIONS;
//     // 6. Configure the sequence of conversions by writing a 5 to SQ1 in the SQR3 register of ADC 3.
//     reg_pointer = (uint32_t*) ADC_3_SQR3_REGISTER;
//     *reg_pointer = (ADC_CHANNEL_5_MORT2 << ADC_SQ1);
//     // 7. Choose 480 cycles of sampling for channel 5 in ADC3 by writing all ones to the SMP7 bits in the ADC3 SMPR2 register.
//     reg_pointer = (uint32_t*)ADC_3_SMPR2_REGISTER;
//     *reg_pointer = ADC_SMP_5_MX;
//     // 8. Turn the ADC on by setting the ADON bit in the CR2 register in ADC 3
//     reg_pointer = (uint32_t*) ADC_3_CR2_REGISTER;
//     *reg_pointer = *reg_pointer | ADC_ADON;
//     // 11. NOTE: A conversion will not start until the SWSTART bit in CR2 bit is set, but we need to wait a few clock cycles for that.
// }