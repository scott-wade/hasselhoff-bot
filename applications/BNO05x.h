#define IMU_CS
#define IMU_INT
#define IMU_RST

// REGISTERS AND COMMANDS
#define CHIP_ID         0x40

// Sensor modes
#define CONFIG_MODE     0x00
#define ACCONLY_MODE    0x01
#define MAGONLY_MODE    0x02
#define GYRONLY_MODE    0x03
#define ACCMAG_MODE     0x04
#define ACCGYRO_MODE    0x05
#define MAGGYRO_MODE    0x06
#define ALL_MODE        0x07
#define IMUPLUS_MODE    0x08
#define COMPASS_MODE    0x09
#define MAG_MODE        0x0A

// Accel ranges
#define ACCEL_2G    0x00
#define ACCEL_4G    0x01
#define ACCEL_8G    0x02
#define ACCEL_16G   0x03
// Accel frequency
#define ACCEL_7_81Hz    0x00 // 7.81 Hz
#define ACCEL_15_63Hz   0x04 // 15.63 Hz
#define ACCEL_31_25Hz   0x08 //31.25 Hz
#define ACCEL_62_5Hz    0x0C
#define ACCEL_125Hz     0x10
#define ACCEL_250Hz     0x14
#define ACCEL_500Hz     0x18
#define ACCEL_1KHz      0x1C
// Accel mode


bool init_IMU();
float get_roll();
float get_pitch();
float get_yaw();


// Global variables
static uint32_t SEQ_NUM = 0;

// Helpers
void hardware_reset();

typedef enum {
    SHTP_COMMAND = 0, // Commands
    SHTP_EXECUTABLE = 1, // Control
    SHTP_SENSOR_HUB=2, // Configuration
    SHTP_SENSOR=3, // Sensor report
    SHTP_WAKE=4, // Wake sensor report
    SHTP_GYRO=5, // Gyro rotation vector
} SHTP_CHANNELS;

typedef struct {
    uint8_t length_lsb;
    uint8_t length_msb;
    uint8_t channel;
    uint8_t sequence_number;
} SHTP_header;

void sendPacket();
void receivePacket();