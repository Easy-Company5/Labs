#include "Mag.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"

// Struct to store magnetic values
Vector3D mag_values;

// Function to initialize the HSCDTD008A geomagnetic sensor
void Mag_init() {
    // Initialize I2C with a 100kHz clock frequency
    i2c_init(i2c1, 100 * 1000);
    // Set SDA and SCL pins to function as I2C
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_set_function(19, GPIO_FUNC_I2C);
    // Enable pull-up resistors on SDA and SCL lines
    gpio_pull_up(18);
    gpio_pull_up(19);
    printf("I2C Initialized on SDA: 18, SCL: 19\n");

    // Initialize the DRDY pin (GPIO20) as an input
    gpio_init(20);
    gpio_set_dir(20, GPIO_IN);  // Set GPIO20 as input for DRDY
    printf("DRDY pin (GPIO20) initialized as input.\n");

    // Set control registers to put the sensor in continuous measurement mode
    uint8_t ctrl1_value = 0;

    // Set FS bit to 0 (continuous measurement), PC bit to 1 (Active mode)
    ctrl1_value |= (1 << 1);  // PC = 1 (Active mode)

    // Set ODR bits to 100Hz (ODR = 0b10)
    ctrl1_value |= (0b10 << 2);  // ODR = 100Hz

    // Write the combined value to the CTRL1 register
    if (Mag_writeRegister(CTRL1_REG, ctrl1_value) != 0) {
        printf("Failed to write CTRL1 register!\n");
        return;
    }
    printf("CNTL1 register set to 0x%02X for continuous measurement and 100Hz ODR.\n", ctrl1_value);

    // Enable DRDY functionality (optional)
    // If you're testing with DRDY, enable it here by writing 0x08 to CTRL2 to activate it.
    if (Mag_writeRegister(CTRL2_REG, 0x08) != 0) {
        printf("Failed to write CTRL2 register to enable DRDY!\n");
        return;
    }
    printf("DRDY enabled with active high signal.\n");
}

// Write to a register
int Mag_writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    if (i2c_write_blocking(i2c1, MAG_I2C_ADDRESS, data, 2, false) != 2) {
        printf("Failed to write to register 0x%X\n", reg);
        return -1;
    }
    return 0;
}

// Read from a register
int Mag_readRegister(uint8_t reg, uint8_t *value) {
    if (i2c_write_blocking(i2c1, MAG_I2C_ADDRESS, &reg, 1, true) != 1) {
        printf("Failed to write register address 0x%X for read!\n", reg);
        return -1;
    }
    if (i2c_read_blocking(i2c1, MAG_I2C_ADDRESS, value, 1, false) != 1) {
        printf("Failed to read from register 0x%X!\n", reg);
        return -1;
    }
    return 0;
}

// Read magnetic field values from the sensor
int Mag_readMagValues() {
    uint8_t reg = OUTX_L_REG;
    uint8_t raw_data[6];

    // Check if data is ready using the DRDY pin (GPIO20)
    if (gpio_get(20)) {  // DRDY is high, meaning data is ready
        // Read 6 bytes of magnetic field data (X, Y, Z axes - 2 bytes each)
        if (i2c_write_blocking(i2c1, MAG_I2C_ADDRESS, &reg, 1, true) != 1) {
            printf("Failed to set register address for reading magnetic field data!\n");
            return -1;
        }
        if (i2c_read_blocking(i2c1, MAG_I2C_ADDRESS, raw_data, 6, false) != 6) {
            printf("Failed to read magnetic field data!\n");
            return -1;
        }

        // Combine the raw data into 16-bit signed values for X, Y, Z axes
        mag_values.x = (int16_t)(raw_data[0] | (raw_data[1] << 8));  // X-axis
        mag_values.y = (int16_t)(raw_data[2] | (raw_data[3] << 8));  // Y-axis
        mag_values.z = (int16_t)(raw_data[4] | (raw_data[5] << 8));  // Z-axis

        return 0;
    } else {
        printf("Data not ready, DRDY pin is low.\n");
        return -1;
    }
}

// Calculate compass heading (in degrees)
float Mag_getHeading() {
    // Heading formula: heading = atan2(Y, X) in radians, then convert to degrees
    float heading = atan2((float)mag_values.y, (float)mag_values.x) * (180.0 / M_PI);

    // Adjust to 0-360 degrees range
    if (heading < 0) {
        heading += 360.0;
    }

    return heading;
}

// Get formatted string of current magnetic field readings
void Mag_getCurrentReadings(char *buf, size_t len) {
    snprintf(buf, len, "X: %d, Y: %d, Z: %d", mag_values.x, mag_values.y, mag_values.z);
}
