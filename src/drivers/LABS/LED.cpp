//LED.cpp
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LED.h"
#include "WS2812.pio.h"
#include "microphone.h"
#include <math.h>  // For ceil and log10
#include "GPIOPins.h"

#define MAX_HUE 360  // Maximum hue value for rainbow
#define TILT_THRESHOLD 5  // Minimum tilt angle in degrees to change LED position
#define MAX_TILT_ANGLE 80
#define BIN_THRESHOLD 1000   // Adjust to control sensitivity

//LAB7
void initialise_pio() {
    PIO pio = pio0; // Use PIO0 for LED control
    uint offset = pio_add_program(pio, &ws2812_program);  // Add WS2812 program to PIO
    int sm = pio_claim_unused_sm(pio, true);  // Claim an unused state machine

    // Initialize the PIO state machine with the correct configurations
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);  // 800kHz frequency, 24-bit colors
}



void initialise_led(uint32_t *led_data, int i, uint8_t sm, uint8_t red, uint8_t green, uint8_t blue){
    if (i < NUM_LEDS) {
        led_data[i] = (red << 24) | (green << 16) | (blue << 8);
    }
}

void update_leds(uint32_t *led_data, int sm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio0, sm, led_data[i]);
    }
}

void turn_off_all(uint32_t *led_data) {
    for (int i = 0; i < NUM_LEDS; i++){
        led_data[i] = 0;
    }
}

// New function to turn all LEDs red
void turnAllRed(uint32_t *led_data, int sm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        initialise_led(led_data, i, sm, 255, 0, 0);  // Red color
    }
    update_leds(led_data, sm);
}

void hsv_to_rgb(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t region = hue / 60;
    uint8_t remainder = (hue - (region * 60)) * 255 / 60;

    uint8_t p = 0;
    uint8_t q = (255 * (60 - remainder)) / 60;
    uint8_t t = (255 * remainder) / 60;

    switch (region) {
        case 0:
            *r = 255; *g = t; *b = p;
            break;
        case 1:
            *r = q; *g = 255; *b = p;
            break;
        case 2:
            *r = p; *g = 255; *b = t;
            break;
        case 3:
            *r = p; *g = q; *b = 255;
            break;
        case 4:
            *r = t; *g = p; *b = 255;
            break;
        case 5:
        default:
            *r = 255; *g = p; *b = q;
            break;
    }
}

void rainbow(uint32_t *led_data, int sm, uint16_t *hue_offset) {
    uint8_t r, g, b;
    uint16_t hue;
    for (int i = 0; i < NUM_LEDS; i++) {
        hue = (*hue_offset + (i * MAX_HUE / NUM_LEDS)) % MAX_HUE;
        hsv_to_rgb(hue, &r, &g, &b);
        initialise_led(led_data, i, sm, r, g, b);
    }
    update_leds(led_data, sm);

    // Increment hue offset for next cycle
    (*hue_offset) += 5;  // Adjust speed by changing the increment value
    if (*hue_offset >= MAX_HUE) {
        *hue_offset = 0;
    }
}

// LAB8
// Function to determine which LED to light based on tilt angle
int mapTiltToLED(float tilt) {
    // Normalize tilt to range 0 to 4 for four LEDs (0: extreme negative tilt, 4: extreme positive tilt)
    int ledIndex = (int)((tilt + MAX_TILT_ANGLE) / (2 * MAX_TILT_ANGLE / 4));
    if (ledIndex < 0) return 0;
    if (ledIndex > 3) return 3;
    return ledIndex;
}

// Function to update LEDs for tilt emulation with paired left and right LEDs
void updateTiltLEDs(uint32_t *led_data, int sm, float xTilt, float yTilt) {
    // Determine which LEDs to light on each side
    int sideLED = mapTiltToLED(xTilt);  // Left and right sides (paired)
    int frontLED = mapTiltToLED(yTilt);  // Front side LEDs (independent)

    // Clear all LEDs
    turn_off_all(led_data);

    // Set LEDs for paired left (red) and right (green) sides based on xTilt
    for (int i = 0; i < 4; i++) {
        if (i == (3 - sideLED)) {  // Reverse the direction for left (red) LEDs
            initialise_led(led_data, i, sm, 0, 255, 0);  // Red for left tilt
        }
        if (i == sideLED) {  // Keep the direction for right (green) LEDs moving "up"
            initialise_led(led_data, i + 8, sm, 0, 255, 0);  // Green for right tilt
        }
    }

    // Set LEDs for front side (blue) independently based on yTilt
    for (int i = 4; i < 8; i++) {
        if (i - 4 == (3 - frontLED)) {  // Reverse the direction for blue (front) LEDs
            initialise_led(led_data, i, sm, 0, 0, 255);  // Blue for forward/backward tilt
        }
    }

    // Update LED display
    update_leds(led_data, sm);
}


void update_leds_based_on_samples(uint32_t *led_data, int sm, q15_t* samples) {
    // Calculate the total volume from FFT samples
    int32_t total_volume = 0;

    // Sum the magnitudes of all FFT samples to get the overall volume
    for (int i = 0; i < FFT_SIZE; i++) {
        total_volume += abs(samples[i]);  // Take the absolute value of each FFT sample and accumulate
    }

    // Scale the total volume to determine how many LEDs to light up
    
    int num_leds_to_light = total_volume / 150000;  

    // Ensure that the number of LEDs to light up is within the range
    if (num_leds_to_light > NUM_LEDS) {
        num_leds_to_light = NUM_LEDS; 
    }
    if (num_leds_to_light < 0) {
        num_leds_to_light = 0;  // Ensure no negative value
    }

for (int i = 0; i < NUM_LEDS; i++) {
    if (i < num_leds_to_light) {
        initialise_led(led_data, i, sm, 255, 0, 255);  // Set to pruple
    } else {
        // Turn off LEDs beyond the volume threshold
        initialise_led(led_data, i, sm, 0, 0, 0);  // set off
    }
}

// Update the LEDs with the new colors
update_leds(led_data, sm);

}


void update_leds_based_on_frequency(uint32_t *led_data, int sm, q15_t* fft_output) {
    // Define the bin ranges corresponding to each LED using logarithmic spacing (suggested by lab)
    const int led_bin_ranges[NUM_LEDS + 1] = {6, 8, 11, 16, 24, 35, 51, 75, 110, 161, 237, 349, 513};

    // Buffer to hold magnitude squared values
    q15_t mag_squared[FFT_OUTPUT_SIZE];

    // Calculate the magnitude squared of each complex number from the FFT output
    arm_cmplx_mag_squared_q15(fft_output, mag_squared, FFT_OUTPUT_SIZE);

    // Find the overall maximum energy value to scale the thresholds dynamically
    q31_t max_energy = 0;
    for (int i = 0; i < FFT_OUTPUT_SIZE; i++) {
        if (mag_squared[i] > max_energy) {
            max_energy = mag_squared[i];
        }
    }

    // Scale the threshold relative to the maximum energy detected
    q31_t dynamic_threshold = max_energy / 1;  // Dynamic threshold based on max energy

    // Loop through each LED
    for (int led_idx = 0; led_idx < NUM_LEDS; led_idx++) {
        int start_bin = led_bin_ranges[led_idx];
        int end_bin = led_bin_ranges[led_idx + 1];

        // Accumulate energy spectral density within the bin range for the current LED
        q31_t energy_sum = 0;
        for (int bin = start_bin; bin < end_bin; bin++) {
            energy_sum += mag_squared[bin];
        }

        // Compare the accumulated energy to the dynamic threshold
        if (energy_sum > dynamic_threshold) {
            initialise_led(led_data, led_idx, sm, 0, 50, 0);  // Set LED color to green
        } else {
            initialise_led(led_data, led_idx, sm, 0, 0, 0);  // Turn off the LED
        }
    }

    // Update the LEDs with the new colors
    update_leds(led_data, sm);
}
