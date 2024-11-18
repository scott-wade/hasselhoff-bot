// #include "BNO05x.h"
// #include "hardware_stm_spi.h"

// void init_IMU()
// {
//     // Confirm that SPI is initialised
//     // If not initialised, initialise
//     // Set RST Pin mode (if applicable)
//     // Set RST High (if applicable)
//     // Set CS Pin mode
//     // Set CS High
// }

// void hardware_reset(int port_number, int pin_number)
// {
//     // RST High
//     SETorCLEARGPIOoutput(port_number, pin_number, 1);
//     // Delay 10 ms
//     delay(10);
//     // RST Low
//     SETorCLEARGPIOoutput(port_number, pin_number, 0);
//     // Delay 10 ms
//     delay(10);
//     // RST High
//     SETorCLEARGPIOoutput(port_number, pin_number, 1);
//     // Delay 10 ms
//     delay(10);
// }

// void sendPacket(SPI_t spi, uint8_t channelNumber, uint8_t *data, uint16_t dataLength)
// {
//     // Send header
//     SHTP_header header;
//     header.length_lsb = dataLength & 0xFF;
//     header.length_msb = dataLength >> 8;
//     header.channel = channelNumber;
//     header.sequence_number = SEQ_NUM++;
//     writeTX(spi, &header)
//         // Send data
//         for (int i = 0; i < dataLength; i++)
//     {
//         writeTX(spi, data[i]);
//     }
// }

// bool receivePacket(float timeout)
// // Blocking subroutine for package reception returns true if package received, false if timeout
// {
//     // Start timer
//     int timeout_counter = 0;
//     while (readGPIOinput(IMU_INT_PORT_NUM, IMU_INT_PIN_NUM) != 0)
//     {
//         // Wait for interrupt
//         delay(1);
//         timeout_counter++;
//         if (timeout_counter > timeout)
//         {
//             // Timeout
//             return false;
//         }
//     }
//     SHTP_header header;
//     header.length_lsb = spiRead();
//     header.length_msb = spiRead();
//     header.channel = spiRead();
//     header.sequence_number = spiRead();

//     // Read data
//     uint16_t packet_size = header.length_lsb | (header.length_msb << 8);
//     // Clear continuity bit
//     header.sequence_number = header.sequence_number & ~(1 << 15);
//     if (packet_size > 0)
//     {
//         uint8_t data[packet_size - 4]; // Removing size of header
//         for (int i = 0; i < packet_size; i++)
//         {
//             data[i] = spiRead();
//         }
//         processPacket(data, packet_size);
//         return true;
//     }
//     else
//     {
//         return false; // Empty packet
//     }
// }

// void processPacket(uint8_t *data, uint16_t data)
// {
//     uint16_t word1 = (uint16_t)data[1] << 8 | data[0]; // Words are 16bit subsections for sensor reports
//     uint16_t word2 = (uint16_t)data[3] << 8 | data[2];
//     uint16_t word3 = (uint16_t)data[5] << 8 | data[4];
// }