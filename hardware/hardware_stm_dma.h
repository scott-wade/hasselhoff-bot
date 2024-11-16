/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_STM_DMA_H_
#define __HARDWARE_STM_DMA_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Constants ------------------------------------------------------------------*/

#define DMA2_BASE_ADDRESS       ((uint32_t) 0x40026400)
#define DMA2_LISR_REGISTER      (DMA2_BASE_ADDRESS + 0x00)
#define DMA2_HISR_REGISTER      (DMA2_BASE_ADDRESS + 0x04)
#define DMA2_LIFCR_REGISTER     (DMA2_BASE_ADDRESS + 0x08)
#define DMA2_HIFCR_REGISTER     (DMA2_BASE_ADDRESS + 0x0C)
#define DMA2_S0CR_REGISTER      (DMA2_BASE_ADDRESS + 0x10)
#define DMA2_S0NDTR_REGISTER    (DMA2_BASE_ADDRESS + 0x14)
#define DMA2_S0PAR_REGISTER     (DMA2_BASE_ADDRESS + 0x18)
#define DMA2_S0M0AR_REGISTER    (DMA2_BASE_ADDRESS + 0x1C)
#define DMA2_S0M1AR_REGISTER    (DMA2_BASE_ADDRESS + 0x20)
#define DMA2_S0FCR_REGISTER     (DMA2_BASE_ADDRESS + 0x24)

// Bits and flags
// DMA SxCR: Dma stream x configuration register
#define DMA_SxCR_CHANNEL_2_SELECT   (((uint32_t)2)<<25) // Select channel 2
#define DMA_SxCR_MSIZE_HALF_WORD    (((uint32_t)1)<<13) // The location memory is 16 bits in length 
#define DMA_SxCR_PSIZE_HALF_WORD    (((uint32_t)1)<<11) // The peripheral data size is 16 bits in length
#define DMA_SxCR_MINC_INCREMENT     (((uint32_t)1)<<10) // Increment the place where you store the data each transfer
#define DMA_SxCR_CIRC_ENABLE        (((uint32_t)1)<<8) // ENABLE circular mode
#define DMA_SxCR_DIR_PERTOMEM       (uint32_t)0
#define DMA_SxCR_STREAM_ENABLE      (uint32_t)1

/* Function definitions ---------------------------------------------------------*/




#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H */


