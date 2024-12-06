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
#include "spi_queue.h"
#include "../applications/packet.h"

/* MACROS for everyone--------------------------------------------------------*/


/* Data structures for SPI state machine */
typedef struct{
    uint8_t child_id; //SPI ID
    uint16_t tx_packet;
    uint8_t* read_var_addr;
} transmitEvent;

typedef enum{
    NUCLEO_PARENT, // The remote
    NUCLEO_CHILD,
    SENSOR_PARENT
} Spi_State_Machine_t;


#define REMOTE_SPI_CHILD_ID 0

/*Function definitions---------------------------------------------------------*/
void init_state_machine_spi(Spi_State_Machine_t);
void event_handler_spi(Spi_State_Machine_t spi_type);
void requestSpiTransmit(Spi_State_Machine_t spi_type, uint8_t child_id, 
    uint16_t packet, uint8_t* read_var_addr);
void requestSpiTransmit_remote(packet_type_t msg_type, uint8_t data, uint8_t* read_var_addr);


#ifdef __cplusplus
}
#endif

#endif /*__STATE_MACHINE_SPI_H */
