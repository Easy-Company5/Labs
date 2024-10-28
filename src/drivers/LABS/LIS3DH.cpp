#include "LIS3DH.h"
#include <stdio.h>
#include <math.h>  // Include math library for atan2
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "GPIOPins.h"

// I2C configuration and conversion constants
 // SCL pin for I2C communication
#define I2C_ADDRESS 0x19  // I2C address of the LIS3DH accelerometer
#define START_ADDR 0x28  // Starting address for reading acceleration data
#define ACC_CONVERSION_RATIO (32768 / 2)  // Conversion ratio for ±2g full scale

// Struct to store accelerometer values
Vector3D values;  // Define the global variable declared as extern in the header

// Function to initialize the LIS3DH accelerometer
void LIS3DH_init() {
    // Initialize I2C with a 100kHz clock frequency
    i2c_init(I2C_INSTANCE, 100 * 1000);
    // Set the SDA and SCL pins to function as I2C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    // Enable pull-up resistors on SDA and SCL lines
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    printf("I2C Initialized on SDA: %d, SCL: %d\n", SDA_PIN, SCL_PIN);

    // Configure accelerometer
    LIS3DH_writeRegister(0x20, 0x57);  // CTRL_REG1: Set data rate to 100Hz and enable all axes
    LIS3DH_writeRegister(0x23, 0x00);  // CTRL_REG4: Set full-scale range to ±2g (00 in FS bits)
}

// Getter function for ACC_CONVERSION_RATIO
float getAccConversionRatio() {
    return ACC_CONVERSION_RATIO;
}

// Write to an accelerometer register
int LIS3DH_writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};  // Create a data array with register address and value to write
    // Write data to the specified register over I2C
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, data, 2, false) != 2) {
        printf("Failed to write to register 0x%X\n", reg);  // Print error if write fails
        return -1;
    }
    return 0;  // Return 0 on success
}

// Read from an accelerometer register
int LIS3DH_readRegister(uint8_t reg, uint8_t *value) {
    // Write the register address we want to read from
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to write register address for read (0x%X)!\n", reg);  // Error if write fails
        return -1;
    }
    // Read the value from the specified register
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, value, 1, false) != 1) {
        printf("Failed to read register value from 0x%X!\n", reg);  // Error if read fails
        return -1;
    }
    return 0;  // Return 0 on success
}

// Read raw acceleration values from the accelerometer
int LIS3DH_readAccelValues() {
    uint8_t reg = START_ADDR | 0x80;  // Set MSB for multi-byte read (auto-increment address)
    uint8_t raw_data[6];  // Array to store the raw data read from accelerometer registers

    // Set the register address to start reading acceleration data
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to set register address for acceleration read!\n");  // Error if write fails
        return -1;
    }
    // Read 6 bytes of acceleration data from LIS3DH (X, Y, Z axes - 2 bytes each)
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, raw_data, 6, false) != 6) {
        printf("Failed to read acceleration data!\n");  // Error if read fails
        return -1;
    }

    // Combine the raw data into 16-bit signed values for x, y, z axes
    values.x = (int16_t)(raw_data[0] | (raw_data[1] << 8));  // X-axis
    values.y = (int16_t)(raw_data[2] | (raw_data[3] << 8));  // Y-axis
    values.z = (int16_t)(raw_data[4] | (raw_data[5] << 8));  // Z-axis

    return 0;  // Return 0 on success
}

// Function to calculate X tilt (roll) in degrees
float getXTilt() {
    // Calculate roll (tilt around X-axis) in degrees using atan2 function
    return atan2((float)values.y, (float)values.z) * (180.0 / M_PI);
}

// Function to calculate Y tilt (pitch) in degrees
float getYTilt() {
    // Calculate pitch (tilt around Y-axis) in degrees using atan2 function
    return atan2((float)values.x, (float)values.z) * (180.0 / M_PI);
}

// Get formatted string of current accelerometer readings
std::string LIS3DH_getCurrentReadings() {
    // Convert raw acceleration values to 'g' units by dividing by conversion ratio
    float x_frac = (float)values.x / ACC_CONVERSION_RATIO;
    float y_frac = (float)values.y / ACC_CONVERSION_RATIO;
    float z_frac = (float)values.z / ACC_CONVERSION_RATIO;

    char buf[50];  // Buffer to store formatted string
    // Format the readings into a string representation with 2 decimal places

}

// Update accelerometer readings
int LIS3DH_updateReadings() {
    return LIS3DH_readAccelValues();  // Read and update the acceleration values from LIS3DH
}
