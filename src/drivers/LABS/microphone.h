#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "pico/stdlib.h"
#include "arm_math.h"

// Define constants
#define FFT_SIZE 1024  // Number of samples to capture
#define FFT_OUTPUT_SIZE (FFT_SIZE / 2)  // Output size of the FFT

class Microphone {
public:
    // Initializes the microphone for reading and configures the ADC
    void init();

    // Reads samples into the provided buffer
    void read(uint16_t* buffer, size_t sample_count);

    // Applies the Hanning window to samples and returns the modified samples
    q15_t* apply_hanning_window();

    // Performs FFT on the sampled data and stores results in the output buffer
    void perform_fft();

    // Buffer to hold the FFT output (real and imaginary parts)
    q15_t fft_output[FFT_SIZE];

    // Getter function to access FFT output
    q15_t* get_fft_output();  // Returns pointer to the FFT output

private:
    // Buffer to hold the modified samples after applying the Hanning window
    q15_t sample_buffer[FFT_SIZE];



    // FFT instance for performing the FFT computation
    arm_rfft_instance_q15 rfft_instance;
};

#endif // MICROPHONE_H
