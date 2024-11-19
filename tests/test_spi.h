#ifndef __TEST_SPI_H_
#define __TEST_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

void testReadWriteRegOpMode(void);
void testReadRegOpMode(void);
void testSPIStateMachine(void);
void testSPIQueue(void);

#ifdef __cplusplus
}
#endif

#endif /*__TEST_SPI_H_ */
