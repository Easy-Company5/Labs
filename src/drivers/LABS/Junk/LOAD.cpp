//LOAD.cpp

#include "LOAD.h"
#include "GPIO_PINS.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Constructor
LoadCell::LoadCell() {
}

// Destructor
LoadCell::~LoadCell() {
}

// Initialize the Load Cell by configuring GPIO pins
void LoadCell::begin() {
    gpio_init(LOAD_SCK);  // Initialize SCK pin
    gpio_set_dir(LOAD_SCK, GPIO_OUT);
    gpio_init(LOAD_DOUT);    // Initialize DOUT pin
    gpio_set_dir(LOAD_DOUT, GPIO_IN);
}



// Wait until the HX711 is ready for reading
void LoadCell::wait_ready() {
    while (gpio_get(LOAD_DOUT) == 1) {
        // DOUT is high; HX711 is not ready
        sleep_us(10);
    }
}

// Read a 24-bit value from the HX711
long LoadCell::read() {
    wait_ready();  // Ensure the HX711 is ready for reading

    long count = 0;

    // Read 24 bits from DOUT, MSB first
    for (int i = 0; i < 24; i++) {
        gpio_put(LOAD_SCK, 1);  // Set SCK high
        count = count << 1;   // Shift bits to the left to make space for the new bit
        gpio_put(LOAD_SCK, 0);  // Set SCK low

        if (gpio_get(LOAD_DOUT)) {  // If DOUT is high, add 1 to count
            count++;
        }
    }

    // Pulse clock pin once more to set gain for next read
    gpio_put(LOAD_SCK, 1);
    count = count ^ 0x800000;  // Convert 24-bit two's complement to signed
    gpio_put(LOAD_SCK, 0);

    return count;
}

// Set the current weight as tare (zero)
void LoadCell::tare() {
    long tareValue = read();
    set_offset(tareValue);
}

// Set and get the scale factor
void LoadCell::set_scale(float scale) {
    SCALE = scale;
}

float LoadCell::get_scale() {
    return SCALE;
}

// Set and get the offset value
void LoadCell::set_offset(long offset) {
    OFFSET = offset;
}

long LoadCell::get_offset() {
    return OFFSET;
}

// Get the weight in units based on scale and offset
float LoadCell::get_units() {
    return (read() - OFFSET) / SCALE;
}
