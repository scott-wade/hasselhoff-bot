#ifndef __TEST_SPI_H_
#define __TEST_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "../state_machine/state_machine_SPI.h"

void testReadWriteRegOpMode(void);
void testReadRegOpMode(void);
void testSPIStateMachine(void);
void testSPIQueue(void);
void testNucleoTransmitting(Spi_State_Machine_t spi_type);
void testNucleoReceiving(Spi_State_Machine_t spi_type);

#ifdef __cplusplus
}
#endif

#endif /*__TEST_SPI_H_ */
