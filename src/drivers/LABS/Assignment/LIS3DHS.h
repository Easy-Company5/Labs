#ifndef LIS3DHAS_H
#define LIS3DHAS_H

#include <string>
#include <stdint.h>  // For uint8_t, int16_t types

// Struct to store accelerometer values
struct Vector3D {
    int16_t x;
    int16_t y;
    int16_t z;
};

// Declare global variable `values` as extern to use in other files
extern Vector3D values;  // Declare the accelerometer values struct as extern

// LIS3DH accelerometer functions
void LIS3DH_init();  // Initializes the LIS3DH accelerometer
void LIS3DH_deinit();  // Deinitializes the LIS3DH accelerometer
int LIS3DH_writeRegister(uint8_t reg, uint8_t value);  // Writes to a register
int LIS3DH_readRegister(uint8_t reg, uint8_t *value);  // Reads from a register
int LIS3DH_readAccelValues();  // Reads raw acceleration values
std::string LIS3DH_getCurrentReadings();  // Returns formatted string of current readings
int LIS3DH_updateReadings();  // Updates internal readings
float getXTilt();  // Calculates the X tilt (roll) in degrees
float getYTilt();  // Calculates the Y tilt (pitch) in degrees

// Getter function for ACC_CONVERSION_RATIO
float getAccConversionRatio();  // Returns the conversion ratio for accelerometer readings
bool check_for_tip();

#endif  // LIS3DHAss_H
