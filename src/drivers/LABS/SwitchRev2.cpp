#include <stdio.h>
#include <array>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <iostream>
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/WS2812/WS2812.h"
#include "drivers/LABS/LIS3DH.h"
#include "drivers/LABS/MicrophoneRev2.h"
#include "colours.h"
#include "GPIOPins.h"
#include "arm_math.h"

void audio_visualiser(Microphone microphone, LedArray led_array)
{
    uint16_t microphone_samples[1024]{};
    microphone.read(microphone_samples);
    int16_t time_domain[1024]{};
    microphone.apply_dc_offset(microphone_samples, time_domain);
    microphone.apply_window_function(time_domain);

    q15_t freq_domain[1026]{};
    microphone.apply_fft(time_domain, freq_domain);

    q15_t cplx_mag_domain[1026]{};
    microphone.get_complex_magnitude(freq_domain, cplx_mag_domain);

    uint32_t bin[12]{};
    microphone.process_results(cplx_mag_domain, bin);

    for (int i{}; i < 12; i++)
    {
        bin[i] <<= 8;
    }
    for (int i{}; i < 12; i++)
    {
        led_array.set(i, bin[i]);
    }

    // led_array.set_multiple(0, bin_6, 1, bin_8, 2, bin_11, 3, bin_16, 4, bin_24, 5, bin_35, 6, bin_51, 7, bin_75, 8, bin_110, 9, bin_161, 10, bin_237, 11, bin_349, 12, bin_513);
    led_array.update();
}
