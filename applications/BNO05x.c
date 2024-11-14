#include "BNO05x.h"

void init_IMU()
{
    // Confirm that SPI is initialised
    // If not initialised, initialise
    // Set RST Pin mode (if applicable)
    // Set RST High (if applicable)
    // Set CS Pin mode
    // Set CS High
}

void hardware_reset()
{
    // RST High
    // Delay 10 ms
    // RST Low
    // Delay 10 ms
    // RST High
    // Delay 10 ms
}

void sendPacket(uint8_t channelNumber, uint8_t dataLength)
{
    // Send header
    // Send data
}

bool receivePacket(float timeout)
// Blocking subroutine for package reception returns true if package received, false if timeout
{
    // Start timer
    int timeout_counter = 0;
    while (readGPIOinput(IMU_INT_PORT_NUM, IMU_INT_PIN_NUM) != 0)
    {
        // Wait for interrupt
        delay(1);
        timeout_counter++;
        if (timeout_counter > timeout)
        {
            // Timeout
            return false;
        }
    }
    SHTP_header header;
    header.length_lsb = spiRead();
    header.length_msb = spiRead();
    header.channel = spiRead();
    header.sequence_number = spiRead();

    // Read data
    uint16_t packet_size = header.length_lsb | (header.length_msb << 8);
    // Clear continuity biy
    header.sequence_number = header.sequence_number & ~(1 << 15);
    if (packet_size > 0)
    {
        uint8_t data[packet_size - 4]; // Removing size of header
        for (int i = 0; i < packet_size; i++)
        {
            data[i] = spiRead();
        }
        processPacket(data);
        return true;
    }
    else
    {
        return false; // Empty packet
    }
}