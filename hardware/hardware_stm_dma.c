#include "stm32f4xx_rcc_mort.h"
#include "hardware_stm_adc.h"
#include "hardware_stm_dma.h"
#include <stdio.h>


/*
 * Initialize DMA for use with ADC
 * @param dma_number: select either DMA1 or DMA2
 * @param dma_channel: the dma channel that corresponds to the ADC, eg. DMA 2 stream 0 channel 2 for ADC3
 * @param adc_number: 1-3, which ADC this DMA is configured for
 * @param num_transfers: the number of transfers that DMA will do
 * @param dest_addr: destination address to store the value into
 */
void initDMAForAdc( int dma_number, int dma_channel, int adc_number, int num_transfers, uint32_t dest_addr ) {
    uint32_t* reg_pointer;
    uint32_t dma_base_address = mapDmaNumbertoBaseAddress(dma_number);
    uint32_t adc_base_address = mapAdcNumbertoBaseAddress(adc_number);

    // 1. Enable the AHB1 clock:
    enableAHB1DMAclock(dma_number);
    // 2. Configure DMA Stream 0 to use channel # (ADC#) with memory size of 
    //      Half Word (16 bits), Peripheral size of Half Word (16 bits),
    //      Increment the source memory address after each transfer, do peripheral 
    //      to memory transfers, enable the DMA circular buﬀer to go back
    //      to the first memory address after the last transfer has been completed. 
    //      This configuration gets written to the S0CR register in DMA
    reg_pointer = (uint32_t *)(long)(dma_base_address + DMA_S0CR_REGISTER_OFFSET);
    uint32_t DMA_SxCR_CHANNEL_NUM_SELECT = ((uint32_t)dma_channel) << 25;
    *reg_pointer = DMA_SxCR_CHANNEL_NUM_SELECT + DMA_SxCR_MSIZE_HALF_WORD +
                   DMA_SxCR_PSIZE_HALF_WORD + DMA_SxCR_MINC_INCREMENT + 
                   DMA_SxCR_DIR_PERTOMEM + DMA_SxCR_CIRC_ENABLE;
    // 3. Configure the S0NDTR to perform # transfers
    reg_pointer = (uint32_t *)(long)(dma_base_address + DMA_S0NDTR_REGISTER_OFFSET);
    *reg_pointer = num_transfers;
    // 4. Write the address of the ADC Data Register in the DMA S0PAR Register to
    //      tell it to transfer from that address.
    reg_pointer = (uint32_t *)(long)(dma_base_address + DMA_S0PAR_REGISTER_OFFSET);
    uint32_t ADC_DR_REGISTER = dma_base_address + ADC_DR_REGISTER_OFFSET;
    *reg_pointer = ADC_DR_REGISTER;
    // 5. Write the address of a buﬀer where you can store the values transfered in memory 
    //      to the S0M0AR register so the DMA stream 0 knows to transfer data to that buﬀer.
    reg_pointer = (uint32_t *)(long)(dma_base_address + DMA_S0M0AR_REGISTER_OFFSET);
    *reg_pointer = dest_addr;
    // ---------------------
    // 6. Enabled the DMA
    reg_pointer = (uint32_t *)(long)(dma_base_address + DMA_S0CR_REGISTER_OFFSET);
    *reg_pointer = *reg_pointer | DMA_SxCR_STREAM_ENABLE;
}


/* *******************************************************************************
                    DMA UTILITY FUNCTIONS
   ******************************************************************************* */
/*
 * Initialize DMA clock
 * @param dma_number: 1-2 for the DMA to select
 */
void enableAHB1DMAclock(int dma_number)
{
    switch (dma_number) {
        case 1 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); break;}
        case 2 : {RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); break;}
        default : fprintf(stderr, "Received Unknown DMA Number at AHB1 clock enable");
    }

}
uint32_t mapDmaNumbertoBaseAddress(int dma_number)
{
    uint32_t dma_base_address;
    switch (dma_number) {
        case 1 : {dma_base_address = DMA1_BASE_ADDRESS; break;}
        case 2 : {dma_base_address = DMA2_BASE_ADDRESS; break;}
        default : fprintf(stderr, "Received Unknown Port Number at Base Address Map");
    }
    return dma_base_address;
}