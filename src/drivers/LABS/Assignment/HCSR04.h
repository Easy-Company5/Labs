// HCSR04.h

#ifndef HCSR04_H
#define HCSR04_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Distance threshold in centimeters
#define DISTANCE_THRESHOLD 10 

// Declare functions for the ultrasonic sensor
void measure_trig_pulse_duration();
float get_distance();


#endif // HCSR04_H
