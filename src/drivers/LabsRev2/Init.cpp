#include "drivers/LabsRev2/Libraries.h"

// Microphone mic;

void Initialise(){
    
    // Initialize the standard IO for the RP2040
    stdio_init_all();

    gpio_init(SW1);
    gpio_set_dir(SW1, GPIO_IN);
    // Setup the switch GPIO for input with a pull-up resistor
    gpio_init(SW1);
    gpio_set_dir(SW1, GPIO_OUT);
    gpio_pull_up(SW1);

    uint8_t current_state{1};

    Microphone microphone;
    microphone.init();

}