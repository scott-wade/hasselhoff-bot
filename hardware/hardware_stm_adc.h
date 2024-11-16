/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_ADC_H_
#define __HARDWARE_STM_ADC_H_

#ifdef __cplusplus
 extern "C" {
#endif


/* Definitions ------------------------------------------------------------------*/

#define ADC_BASE_ADDRESS        ((uint32_t)0x40012000) // 0x4001 2000 - 0x4001 23FF
#define ADC_1_BASE_ADDRESS      (ADC_BASE_ADDRESS +  0x000) // 0x000 - 0x04C
#define ADC_2_BASE_ADDRESS      (ADC_BASE_ADDRESS +  0x100) // 0x100 - 0x14C
#define ADC_3_BASE_ADDRESS      (ADC_BASE_ADDRESS +  0x200) // 0x200 - 0x24C
#define ADC_3_SR_REGISTER       (ADC_3_BASE_ADDRESS + 0x00)
#define ADC_3_CR1_REGISTER      (ADC_3_BASE_ADDRESS + 0x04)
#define ADC_3_CR2_REGISTER      (ADC_3_BASE_ADDRESS + 0x08)
#define ADC_3_SMPR1_REGISTER    (ADC_3_BASE_ADDRESS + 0x0C)
#define ADC_3_SMPR2_REGISTER    (ADC_3_BASE_ADDRESS + 0x10)
#define ADC_3_SQR1_REGISTER     (ADC_3_BASE_ADDRESS + 0x2C)
#define ADC_3_SQR2_REGISTER     (ADC_3_BASE_ADDRESS + 0x30)
#define ADC_3_SQR3_REGISTER     (ADC_3_BASE_ADDRESS + 0x34)
#define ADC_3_DR_REGISTER       (ADC_3_BASE_ADDRESS + 0x4C)
#define ADC_COMMON_CCR_REGISTER (ADC_1_BASE_ADDRESS + 0x300 + 0x04) // Address offset: 0x04 (this offset address is relative to ADC1 base address + 0x300)
#define ADC_EOC                 (uint32_t)(0x01<<1) // Bit 1 EOC: Regular channel end of conversion
#define ADC_ALIGN               (uint32_t)(0x01<<11) // Bit 11 ALIGN: Data alignment
#define ADC_EOCS                (uint32_t)(0x01<<10) // Bit 10 EOCS: End of conversion selection
#define ADC_DDS                 (uint32_t)(0x01<<9) // Bit 9 DDS: DMA disable selection (for single ADC mode)
#define ADC_DMA                 (uint32_t)(0x01<<8) // Bit 8 DMA: Direct memory access mode (for single ADC mode)
#define ADC_CONT                (uint32_t)(0x01<<1) // Bit 1 CONT: Continuous conversion
#define ADC_ADON                (uint32_t)(0x01<<0) // Bit 0 ADON: A/D Converter ON / OFF
#define ADC_SCAN                (uint32_t)(0x01<<8) // Bit 8 SCAN: Scan mode
#define ADC_PRESCALER_4         (uint32_t)(0x01<<16) // At Bits 17:16, 01: PCLK2 divided by 4
#define ADC_1_CONVERSIONS       (uint32_t)(0b0000<<20) // 0000: 1 conversion at Bits 23:20 L[3:0]: Regular channel sequence length
#define ADC_2_CONVERSIONS       (uint32_t)(0b0001<<20) // 0001: 2 conversions at Bits 23:20 L[3:0]: Regular channel sequence length   
#define ADC_3_CONVERSIONS       (uint32_t)(0b0010<<20) // 0010: 3 conversions at Bits 23:20 L[3:0]: Regular channel sequence length
#define ADC_CHANNEL_5_MORT2     (uint32_t)(0x05)
#define ADC_CHANNEL_6_MORT2     (uint32_t)(0x06)
#define ADC_CHANNEL_7_MORT2     (uint32_t)(0x07)
#define ADC_SQ1                 0   // Bits 4:0 SQ1[4:0]: 1st conversion in regular sequence
#define ADC_SQ2                 5   // Bits 9:5 SQ2[4:0]: 2nd conversion in regular sequence
#define ADC_SQ3                 10  // Bits 14:10 SQ3[4:0]: 3rd conversion in regular sequence
#define ADC_SWSTART             (uint32_t)(0x01<<30) // Bit 30 SWSTART: Start conversion of regular channels
#define ADC_SMP_5_MX            (uint32_t)(0b111<<(5*3))
#define ADC_SMP_6_MX            (uint32_t)(0b111<<(6*3))
#define ADC_SMP_7_MX            (uint32_t)(0b111<<(7*3))

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Function definitions ---------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */
