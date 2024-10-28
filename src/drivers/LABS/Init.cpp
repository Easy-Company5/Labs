#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "drivers/LABS/Switch.h"
#include "drivers/LABS/Microphone.h"
#include "drivers/LABS/LED.h"
#include "drivers/LABS/Misc.h"
#include "drivers/LABS/LIS3DH.h"

Microphone mic;

void Initialise(){
    
    // Initialize the standard IO for the RP2040
    stdio_init_all();

    // Initialize switch and other peripherals
    switch_init();
    mic.init();         // Initialize the microphone
    LIS3DH_init();      // Initialize the accelerometer
    initialise_pio();         // Initialize the LEDs

}