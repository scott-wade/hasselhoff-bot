#pragma once

typedef enum
{
    SHTP_COMMAND = 0,    // Commands
    SHTP_EXECUTABLE = 1, // Control
    SHTP_SENSOR_HUB = 2, // Configuration
    SHTP_SENSOR = 3,     // Sensor report
    SHTP_WAKE = 4,       // Wake sensor report
    SHTP_GYRO = 5,       // Gyro rotation vector
} SHTP_CHANNELS;

typedef struct
{
    uint8_t length_lsb;
    uint8_t length_msb;
    uint8_t channel;
    uint8_t sequence_number;
} SHTP_header;
