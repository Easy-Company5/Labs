#pragma once

#define LED_PIN 14
#define NUM_LEDS 12
#define SDA_PIN 16  // SDA pin for I2C communication
#define SCL_PIN 17 

#define SW1 15      // Define the GPIO pin number for the switch


//Accelerometer
#define I2C_INSTANCE i2c0  // I2C instance to use 
#define I2C_ADDRESS 0x19  // I2C address of the LIS3DH accelerometer
#define START_ADDR 0x28  // Starting address for reading acceleration data

#define NUMBER_OF_TASKS 3