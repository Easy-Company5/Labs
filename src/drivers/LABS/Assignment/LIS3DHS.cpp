#include "LIS3DHS.h"
#include <stdio.h>
#include <math.h>  // Include math library for atan2
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "GPIO_PINS.h"

// I2C configuration and conversion constants
#define I2C_INSTANCE i2c0  // Use I2C instance 0 for communication
#define I2C_ADDRESS 0x18  // I2C address for the LIS3DH accelerometer
#define START_ADDR 0x28  // Starting register address for reading accelerometer data
#define ACC_CONVERSION_RATIO (32768 / 2)  // Conversion ratio for accelerometer (±2g full scale)

// Define the range of acceptable g-force thresholds for tilt detection
#define G_THRESHOLD_HIGH 1.5  // Upper limit for g-force
#define G_THRESHOLD_LOW 0.5  // Lower limit for g-force

// Define the acceptable bounds for accelerometer readings on X, Y, Z axes
#define X_LOWER_BOUND -0.07
#define X_UPPER_BOUND 0.07
#define Y_LOWER_BOUND -0.07
#define Y_UPPER_BOUND 0.07
#define Z_LOWER_BOUND 0.93
#define Z_UPPER_BOUND 1.07

// Struct to store accelerometer values (global variable)
Vector3D values;  // Define the global variable declared as extern in the header

// Function to initialize the LIS3DH accelerometer
void LIS3DH_init() {
    // Initialize I2C communication with the defined instance and baud rate
    i2c_init(I2C_INSTANCE, 100 * 1000);  // Set I2C baud rate to 100kHz
    gpio_set_function(ACCEL_SDA, GPIO_FUNC_I2C);  // Set SDA pin for I2C
    gpio_set_function(ACCEL_SCL, GPIO_FUNC_I2C);  // Set SCL pin for I2C
    gpio_pull_up(ACCEL_SDA);  // Enable pull-up resistor on SDA pin
    gpio_pull_up(ACCEL_SCL);  // Enable pull-up resistor on SCL pin
    printf("I2C Initialized on SDA: %d, SCL: %d\n", ACCEL_SDA, ACCEL_SCL);

    // Configure the accelerometer by writing to its control registers
    LIS3DH_writeRegister(0x20, 0x57);  // CTRL_REG1: Enable all axes, set data rate to 100Hz
    LIS3DH_writeRegister(0x23, 0x00);  // CTRL_REG4: Set full scale range to ±2g
}

// Function to get the accelerometer conversion ratio (g-force)
float getAccConversionRatio() {
    return ACC_CONVERSION_RATIO;
}

// Write a value to an accelerometer register
int LIS3DH_writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};  // Prepare data for writing (register address + value)
    
    // Write to the register via I2C and check for success
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, data, 2, false) != 2) {
        printf("Failed to write to register 0x%X\n", reg);
        return -1;
    }
    return 0;  // Return success
}

// Read a value from an accelerometer register
int LIS3DH_readRegister(uint8_t reg, uint8_t *value) {
    // Write the register address to read from
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to write register address for read (0x%X)!\n", reg);
        return -1;
    }
    // Read the register value from the sensor
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, value, 1, false) != 1) {
        printf("Failed to read register value from 0x%X!\n", reg);
        return -1;
    }
    return 0;  // Return success
}

// Read raw acceleration values from the accelerometer (x, y, z axes)
int LIS3DH_readAccelValues() {
    uint8_t reg = START_ADDR | 0x80;  // Set the MSB for multi-byte read
    uint8_t raw_data[6];  // Buffer to store raw accelerometer data (x, y, z)

    // Set the register address for reading acceleration data
    if (i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to set register address for acceleration read!\n");
        return -1;
    }
    // Read 6 bytes of data (2 bytes each for x, y, z axes)
    if (i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, raw_data, 6, false) != 6) {
        printf("Failed to read acceleration data!\n");
        return -1;
    }

    // Combine the raw data bytes into 16-bit signed integers for x, y, z
    values.x = (int16_t)(raw_data[0] | (raw_data[1] << 8));
    values.y = (int16_t)(raw_data[2] | (raw_data[3] << 8));
    values.z = (int16_t)(raw_data[4] | (raw_data[5] << 8));

    return 0;  // Return success
}

// Function to calculate the X-axis tilt (roll) in degrees
float getXTilt() {
    // Calculate roll based on the y and z axis values using atan2
    return atan2((float)values.y, (float)values.z) * (180.0 / M_PI);
}

// Function to calculate the Y-axis tilt (pitch) in degrees
float getYTilt() {
    // Calculate pitch based on the x and z axis values using atan2
    return atan2((float)values.x, (float)values.z) * (180.0 / M_PI);
}

// Get a formatted string showing current accelerometer readings in g-units
std::string LIS3DH_getCurrentReadings() {
    // Convert raw accelerometer values to g-units
    float x_frac = (float)values.x / ACC_CONVERSION_RATIO;
    float y_frac = (float)values.y / ACC_CONVERSION_RATIO;
    float z_frac = (float)values.z / ACC_CONVERSION_RATIO;

    // Format the string to show the x, y, z values in g-units
    char buf[50];
    snprintf(buf, sizeof(buf), "(x: %.2fg, y: %.2fg, z: %.2fg)", x_frac, y_frac, z_frac);
    return std::string(buf);  // Return the formatted string
}

// Update the accelerometer readings by reading new values from the sensor
int LIS3DH_updateReadings() {
    return LIS3DH_readAccelValues();  // Return the status of the read operation
}

// Function to check if the device has tipped over based on accelerometer data
bool check_for_tip() {
    // Update accelerometer readings
    if (LIS3DH_updateReadings() != 0) {
        printf("Failed to update accelerometer readings\n");
        return false;  // Return false if updating readings failed
    }

    // Convert x and y-axis readings to g-units
    float x_g = (float)values.x / ACC_CONVERSION_RATIO;
    float y_g = (float)values.y / ACC_CONVERSION_RATIO;

    // Print x-axis acceleration for debugging purposes
    printf("X-axis acceleration: %.2fg\n", x_g);

    // Check if the x or y-axis acceleration exceeds predefined thresholds
    if (fabs(x_g) > 0.3 || fabs(y_g) > 0.7) {
        printf("Tip detected! X or Y-axis acceleration exceeds threshold.\n");
        sleep_ms(1000);  // Pause for 1 second after detecting a tip
        return true;  // Return true if a tip is detected
    }

    return false;  // Return false if no tipping is detected
}
