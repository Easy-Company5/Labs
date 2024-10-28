//LOAD.h

#ifndef LOAD_H
#define LOAD_H

#include "pico/stdlib.h"
class LoadCell {
public:
    // Constructor and Destructor
    LoadCell();
    ~LoadCell();

    // Initialize the load cell with GPIO pins
    void begin();

    // Read a 24-bit value from the load cell
    long read();

    // Wait for the HX711 to be ready for reading
    void wait_ready();

    // Tare function to set the current weight as zero
    void tare();

    // Set and get scale factor for weight calculation
    void set_scale(float scale);
    float get_scale();

    // Set and get the offset value for tare weight
    void set_offset(long offset);
    long get_offset();

    // Get the weight in user-defined units
    float get_units();

private:
    long OFFSET = 0;    // Tare weight
    float SCALE = 1.0;  // Scale factor
};

#endif // LOAD_H
