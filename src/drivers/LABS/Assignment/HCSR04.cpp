#include "HCSR04.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "GPIO_PINS.h"

volatile uint32_t rise_time = 0;
volatile uint32_t fall_time = 0;
volatile bool trig_pulse_complete = false;

// Non-blocking measurement interval
static uint32_t last_measure_time = 0;
#define MEASURE_INTERVAL_MS 100  // Measure every 100 ms

// Interrupt handler for echo pin
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == EchoPin) {  // Change to EchoPin for the callback
        if (events & GPIO_IRQ_EDGE_RISE) {
            rise_time = time_us_32();
        }
        if (events & GPIO_IRQ_EDGE_FALL) {
            fall_time = time_us_32();
            trig_pulse_complete = true;  // Signal that the pulse is complete
        }
    }
}

// Non-blocking ultrasonic distance measurement
void trigger_ultrasonic() {
    // Trigger pulse
    gpio_put(TrigPin, 1);
    sleep_us(10);
    gpio_put(TrigPin, 0);
}

void measure_trig_pulse_duration() {
    // Set up the EchoPin as input and wait for the pulse to complete
    gpio_init(EchoPin);
    gpio_set_dir(EchoPin, GPIO_IN);
    gpio_set_irq_enabled_with_callback(EchoPin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    // Configure Trigger Pin
    gpio_init(TrigPin);
    gpio_set_dir(TrigPin, GPIO_OUT);
    gpio_put(TrigPin, 0);  // Ensure it starts low
}

float get_distance() {
    // Trigger the ultrasonic pulse if enough time has passed
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_measure_time >= MEASURE_INTERVAL_MS) {
        trigger_ultrasonic();
        last_measure_time = now;
    }

    // Check if pulse is complete
    if (trig_pulse_complete) {
        uint32_t time_diff_us = fall_time - rise_time;  // Time difference in microseconds
        float distance_cm = (time_diff_us / 2.0) * 0.0343;  // Convert to centimeters
        trig_pulse_complete = false;  // Reset for next measurement
        return distance_cm;
    }

    return -1;  // Return -1 if no valid measurement yet
}
