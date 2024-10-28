// GPIO_PINS.h

#ifndef GPIO_PINS_H
#define GPIO_PINS_H

// Include necessary libraries
#include "pico/stdlib.h"

// GPIO Pin Definitions
#define TX_Pin              0       // UART Transmit (RP2040 to Raspberry Pi)
#define RX_Pin              1       // UART Receive (Raspberry Pi to RP2040)
#define LOAD_DOUT           2       // Load cell data output (HX711)
#define LOAD_SCK            3       // Load cell serial clock (HX711)
#define ACCEL_SDA           4       // Accelerometer I2C data line
#define ACCEL_SCL           5       // Accelerometer I2C clock line
#define ACCEL_Int1          6       // Accelerometer interrupt 1 pin
#define ACCEL_Int2          7       // Accelerometer interrupt 2 pin
#define UART_TX_PIN         8       // Bluetooth module UART Transmit
#define UART_RX_PIN         9       // Bluetooth module UART Receive
#define BLUE_CTS            10      // Bluetooth module CTS (Clear to Send)
#define BLUE_RTS            11      // Bluetooth module RTS (Request to Send)
#define BLUE_Reset          12      // Bluetooth module Reset pin
#define TrigPin             13      // Ultrasonic sensor Trigger pin
#define EchoPin             14      // Ultrasonic sensor Echo pin
#define LED_PN              15      // LED data input pin (for driving LEDs)
#define MAG_SDA             18      // Magnetometer I2C data line
#define MAG_SCL             19      // Magnetometer I2C clock line
#define MAG_Interrupt       20      // Magnetometer interrupt pin
#define STEP_L              21      // Stepper Motor L Step pin
#define DIR_L               22      // Stepper Motor L Direction pin
#define STEP_R              23      // Stepper Motor R Step pin
#define DIR_R               24      // Stepper Motor R Direction pin
#define STEP_B              25      // Stepper Motor B Step pin
#define DIR_B               26      // Stepper Motor B Direction pin
#define STEP_F              27      // Stepper Motor F Step pin
#define DIR_F               28      // Stepper Motor F Direction pin
#define MOTOR_RESET         29      // Motor Reset (shared by all motors)

// UART Settings
#define BAUD_RATE           115200  // UART communication baud rate
#define UART_ID             uart1   // UART hardware ID

// Communication Buffer Settings
#define MAX_BUFFER_SIZE     100     // Maximum size of the communication buffer
#define SEND_INTERVAL_MS    2000    // Interval (in ms) between sending data over UART

// Global Variables
char buffer[MAX_BUFFER_SIZE];        // Buffer for storing incoming data
int buffer_index = 0;                // Index for tracking buffer position
volatile bool string_received = false;  // Flag indicating that a complete string has been received

#endif // GPIO_PINS_H
