
//Switch.cpp
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "drivers/LABS/microphone.h"
#include "drivers/LABS/LED.h"
#include "drivers/LABS/Misc.h"
#include "drivers/LABS/LIS3DH.h"
#include "GPIOPins.h"

static uint32_t led_data[NUM_LEDS] = {0};  // Array to store LED color data
extern Microphone mic;  // Use the external microphone variable defined in main.cpp
int j = 0;
/**
 * @brief Perform a single iteration of Case 1 (Microphone FFT Processing).
 */
void Case1() {
    if (j == 0){
        clearScreen();
        printf("\nExecuting Case 1: Microphone FFT Processing\n");
        j = 1;
    }
    // Perform FFT on the microphone samples and update LEDs
    
    mic.perform_fft();
    q15_t* fft_output = mic.get_fft_output();
    update_leds_based_on_samples(led_data, 0, fft_output);

    
}

/**
 * @brief Perform a single iteration of Case 2 (Accelerometer Tilt Detection).
 */
void Case2() {
        if (j == 1){
        clearScreen();
        printf("\nExecuting Case 2: Accelerometer Tilt Detection\n");
        j = 2;
    }
    // Define the range for X, Y, Z accelerometer readings
    #define X_LOWER_BOUND -0.07
    #define X_UPPER_BOUND 0.07
    #define Y_LOWER_BOUND -0.07
    #define Y_UPPER_BOUND 0.07
    #define Z_LOWER_BOUND 0.93
    #define Z_UPPER_BOUND 1.07

    static bool initialized = false;


    // Read tilt values from the LIS3DH accelerometer
    LIS3DH_updateReadings();
    float xTilt = getXTilt();  // Function to convert LIS3DH data to X tilt angle
    float yTilt = getYTilt();  // Function to convert LIS3DH data to Y tilt angle

    // Get current readings in g units
    float x = (float)values.x / getAccConversionRatio();
    float y = (float)values.y / getAccConversionRatio();
    float z = (float)values.z / getAccConversionRatio();

    // Check if readings are within the specified range
    if (x >= X_LOWER_BOUND && x <= X_UPPER_BOUND &&
        y >= Y_LOWER_BOUND && y <= Y_UPPER_BOUND &&
        z >= Z_LOWER_BOUND && z <= Z_UPPER_BOUND) {
        
        // Turn all LEDs red
        turnAllRed(led_data, 0);
    } else {
        // Update LEDs based on tilt
        updateTiltLEDs(led_data, 0, xTilt, yTilt);
    }

    // Print current readings in g units (for debugging)
    printf("Accelerometer Readings: X=%.2f, Y=%.2f, Z=%.2f\n", x, y, z);

    sleep_ms(100);  // Delay for readability
}

void Case3() {
    if (j == 2){
        clearScreen();
        printf("\nFrequency?\n");
        j = 0;
    }
    mic.perform_fft();
    q15_t* fft_output = mic.get_fft_output();
    update_leds_based_on_frequency(led_data, 0, fft_output);
   
}

void Case4(){

    #define UART_ID uart1               // Use UART1
    #define BAUD_RATE 115200            // Ensure this matches the Raspberry Pi
    #define UART_TX_PIN 8               // TX pin (GPIO )
    #define UART_RX_PIN 9               // RX pin (GPIO 9)
    #define DATA_BITS 8
    #define STOP_BITS 1
    #define PARITY UART_PARITY_NONE
        // Read accelerometer data

    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    LIS3DH_updateReadings();
        // Get current readings in g units
        float x = (float)values.x / getAccConversionRatio();
        float y = (float)values.y / getAccConversionRatio();
        float z = (float)values.z / getAccConversionRatio();

        // Create a buffer to hold the formatted string
        char accel_buffer[100];

        // Format the accelerometer readings into the buffer
        snprintf(accel_buffer, sizeof(accel_buffer), "X: %.2f, Y: %.2f, Z: %.2f\n", x, y, z);

        // Send the formatted string over UART
        uart_puts(UART_ID, accel_buffer);
        }
