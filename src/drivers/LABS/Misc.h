#ifndef MISC_H
#define MISC_H

#include <stdint.h>    // For uint8_t, uint32_t types
#include "pico/stdlib.h"  // For GPIO and other Pico SDK functionalities



// Miscellaneous Utility Functions

/**
 * @brief Clears the console screen using ANSI escape codes.
 */
void clearScreen();

/**
 * @brief Interrupt Service Routine (ISR) for handling switch interrupts.
 * 
 * @param gpio The GPIO pin number that triggered the interrupt.
 * @param events The type of interrupt event that occurred.
 */
void Interrupt(uint gpio, uint32_t events);

/**
 * @brief Initializes the switch (SW1) for input and configures it to trigger an interrupt on falling edge.
 */
void switch_init();

/**
 * @brief Retrieves the current state of the switch.
 * 
 * @return int The current state of the switch, with values ranging from 0 to 3.
 */
int get_switch_state(void);


void i2c_scan();

#endif  // MISC_H
