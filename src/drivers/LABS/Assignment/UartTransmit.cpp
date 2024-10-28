#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include <iostream>
#include <string>
#include <cstring>
#include "GPIO_PINS.h"



// Initialize UART with configured baud rate and pins
void uart_init() {
    uart_init(UART_ID, BAUD_RATE);                  // Initialize UART with specified baud rate
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Set RX pin function to UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Set TX pin function to UART
    uart_set_fifo_enabled(UART_ID, true);           // Enable the FIFO buffer for UART
}

// Receive a character over UART
char uart_receive() {
    if (uart_is_readable(UART_ID)) {                // Check if UART has data available to read
        return uart_getc(UART_ID);                  // Return received character
    }
    return '\0';                                    // Return null character if nothing is readable
}

// Send a message over UART, ending with a newline
void uart_send(const char *message) {
    while (*message) {                              // Loop through each character in the message
        uart_putc(UART_ID, *message++);             // Send each character over UART
    }
    uart_putc(UART_ID, '\n');                       // Append newline to indicate the end of a message
}


// UART receive ISR
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);

        // Check for buffer overflow
        if (buffer_index < MAX_BUFFER_SIZE - 1) {
            buffer[buffer_index++] = ch;
            uart_putc(UART_ID, ch);  // Echo the received character back to the UART
            buffer[buffer_index] = '\0';  // Null-terminate the string
            buffer_index = 0;  // Reset the buffer index for the next character
            string_received = true;  // Set flag to indicate data is ready
        }
    }
}