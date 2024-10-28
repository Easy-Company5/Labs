#ifndef LED_H
#define LED_H

#include <stdint.h>  // For uint8_t, uint16_t types
#include "arm_math.h"  // Include ARM math library for q15_t type

// Define the number of LEDs
#define NUM_LEDS 12


// LED control functions
void initialise_pio();  // Initializes PIO for WS2812 LEDs
void initialise_led(uint32_t *led_data, int i, uint8_t sm, uint8_t red, uint8_t green, uint8_t blue);  // Initializes a specific LED with color and state machine
void update_leds(uint32_t *led_data, int sm);  // Updates all LEDs based on led_data array
void turn_off_all(uint32_t *led_data);  // Turns off all LEDs
void turnAllRed(uint32_t *led_data, int sm);  // Turns all LEDs red

// Function to convert HSV color format to RGB
void hsv_to_rgb(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b);

// Function to display a rainbow effect on the LEDs
void rainbow(uint32_t *led_data, int sm, uint16_t *hue_offset);

// Function to update LEDs based on FFT output values from microphone
void update_leds_based_on_samples(uint32_t *led_data, int sm, q15_t* samples);
void update_leds_based_on_frequency(uint32_t *led_data, int sm, q15_t* fft_output);
// Function to map tilt value to LED index
int mapTiltToLED(float tilt);

// Function to update LEDs for tilt emulation with paired left and right LEDs
void updateTiltLEDs(uint32_t *led_data, int sm, float xTilt, float yTilt);



#endif  // LED_H
