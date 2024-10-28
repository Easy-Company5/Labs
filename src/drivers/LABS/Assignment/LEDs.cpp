#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h"
#include "drivers/logging/logging.h"
#include "drivers/LABS/Assignment/LEDs.h"
#include "GPIO_PINS.h"

#define NUM_LEDS 8

void LED_PIO_init() {
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PN, 800000, false);
}

void LEDs_init(uint32_t *led_data, int i, uint8_t red, uint8_t green, uint8_t blue) {
    if (i < NUM_LEDS) { // Ensure the index is within bounds
        led_data[i] = (red << 24) | (green << 16) | (blue << 8); // Stores the RGB colour into the 'ith' element of 'led_data' (corresponds to the ith LED).
    }
}

void LEDs_update(uint32_t *led_data) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio0, 0, led_data[i]); // Send the color data for each LED.
    }
}

void LEDs_off(uint32_t *led_data) {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_data[i] = 0;  // Set the data for each LED to black (off).
    }
    LEDs_update(led_data); // Run the update unction (defined above) to update the colour of all LED's.
}

void LEDs_blue(uint32_t *led_data){
        for(int i = 0; i < NUM_LEDS; i++){
            LEDs_init(led_data, i, 0, 0, 255);
        }
    LEDs_update(led_data);
}

void LEDs_red(uint32_t *led_data){
        for(int i = 0; i < NUM_LEDS; i++){
            LEDs_init(led_data, i, 255, 0, 0);
        }
    LEDs_update(led_data);
}

void LEDs_green(uint32_t *led_data){
        for(int i = 0; i < NUM_LEDS; i++){
            LEDs_init(led_data, i, 0, 255, 0);
        }
    LEDs_update(led_data);
}