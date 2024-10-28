//Misc.cpp
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "Misc.h"
#include "GPIOPins.h"


// Global variable to store the current switch state (initialized to 0)
static volatile int switchno = 0;

// Function Definitions

/**
 * @brief Clears the console screen using ANSI escape codes.
 */
void clearScreen() {
    printf("\033[2J\033[H");  // ANSI escape code to clear the screen and move the cursor to the top-left corner
}

/**
 * @brief Interrupt handler for switch press. Updates the switch state.
 * 
 * @param gpio The GPIO pin number that triggered the interrupt.
 * @param events The type of interrupt event that occurred.
 */

void Interrupt(uint gpio, uint32_t events) {
    // Increment the switch state and wrap it between 0 and 1
    switchno = (switchno + 1) % 4;
        // Print out the event type (rising or falling)
    // if (events & GPIO_IRQ_EDGE_RISE) {
    //     printf("Rising edge detected on GPIO %d\n", gpio);
    // } else if (events & GPIO_IRQ_EDGE_FALL) {
    //     printf("Falling edge detected on GPIO %d\n", gpio);
    // }

}

/**
 * @brief Initializes the switch (SW1) for input and configures it to trigger an interrupt on falling edge.
 */
void switch_init() {
    gpio_init(SW1);
    gpio_set_dir(SW1, GPIO_IN);
    
    gpio_set_irq_enabled_with_callback(SW1, GPIO_IRQ_EDGE_RISE, true, &Interrupt);
    
}


/**
 * @brief Returns the current state of the switch.
 * 
 * @return int The current state of the switch, with values ranging from 0 to 1.
 */
int get_switch_state(void) {
    return switchno;
}
