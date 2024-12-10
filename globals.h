/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <cstdint>
#include "queue.h"

    /* globals for everyone--------------------------------------------------------*/

    // global variables for spi state machine
    extern uint16_t *SPI_READ_ADDR;      // address to send current RX to
    extern uint32_t SPI_OUTGOING_PACKET; // packet to send in current transaction
    extern uint8_t SPI_TRANSMIT_FLAG;

    // State machine global
    extern simple_queue_t simpleQ;
    extern uint16_t*  rx_packet;


#ifdef __cplusplus
}
#endif

#endif /*__GLOBALS_H */
