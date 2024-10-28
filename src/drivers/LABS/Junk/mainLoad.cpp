#include "drivers/LABS/Assignment/HX711.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

// Define GPIO pins for HX711
#define DOUT_PIN 2  // HX711 DOUT pin
#define CLK_PIN 3   // HX711 CLK pin

HX711 scale;

int mainLoad() {
    stdio_init_all();  // Initialize USB serial output

    // Initialize the HX711
    scale.begin(DOUT_PIN, CLK_PIN);
    scale.set_scale(-22000);  // Set calibration factor
    scale.set_offset(22500);  // Set zero offset (tare)

    while (true) {
        // Read weight and print to USB serial
        float weight = scale.get_units();
        printf("Weight: %.2f g\n", weight);
        sleep_ms(1000);  // Wait 1 second
    }
}
