#ifndef MAG_H
#define MAG_H

#include <stdint.h>
#include <stdio.h>

// I2C Configuration
#define I2C_INSTANCE i2c1          // I2C instance to use (i2c1)
#define MAG_I2C_ADDRESS 0x0C       // Fixed I2C address for HSCDTD008A sensor

// Sensor Registers
#define WHO_AM_I_REG 0x0F          // WHO_AM_I register
#define OUTX_L_REG 0x10            // X-axis output data LSB
#define OUTX_H_REG 0x11            // X-axis output data MSB
#define OUTY_L_REG 0x12            // Y-axis output data LSB
#define OUTY_H_REG 0x13            // Y-axis output data MSB
#define OUTZ_L_REG 0x14            // Z-axis output data LSB
#define OUTZ_H_REG 0x15            // Z-axis output data MSB
#define CTRL1_REG 0x1B             // Control register 1 (for mode, ODR)
#define CTRL2_REG 0x1C             // Control register 2 (for interrupts, etc.)

// Function Prototypes
void Mag_init();                          // Function to initialize the magnetometer
int Mag_readRegister(uint8_t reg, uint8_t *value);  // Read from a register
int Mag_writeRegister(uint8_t reg, uint8_t value);  // Write to a register
int Mag_readMagValues();                  // Read magnetic field values (X, Y, Z)
void Mag_getCurrentReadings(char *buf, size_t len); // Get formatted string of current magnetic field readings
float Mag_getHeading();                   // Function to calculate compass heading

// Struct to store magnetic values
struct Vector3D {
    int16_t x;
    int16_t y;
    int16_t z;
};

extern Vector3D mag_values;

#endif
