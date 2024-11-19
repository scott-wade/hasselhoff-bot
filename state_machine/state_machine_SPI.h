/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_SPI_H_
#define __STATE_MACHINE_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <cstdint>
#include "queue.h"
#include "main.h"

/* MACROS for everyone--------------------------------------------------------*/


/* Data structures for SPI state machine */
typedef struct{
    uint8_t child_id; //SPI ID
    uint32_t txQueue;
    uint32_t* read_var_addr;
} transmitEvent;

typedef struct{
    uint16_t recieved_packet;
}recieveEvent;

/*Function definitions---------------------------------------------------------*/
void init_state_machine_spi(void);
void event_handler_spi(void);
void requestSpiTransmit(uint8_t child_id, uint16_t packet, uint16_t* read_var_addr);



#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_SPI_H */
