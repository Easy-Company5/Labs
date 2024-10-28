#ifndef HX711_h
#define HX711_h

#include "pico/stdlib.h"  // Use RP2040 standard library

class HX711 {
    private:
        uint8_t PD_SCK;   // Power Down and Serial Clock Input Pin
        uint8_t DOUT;     // Serial Data Output Pin
        uint8_t GAIN;     // Amplification factor
        long OFFSET = 0;  // Used for tare weight
        float SCALE = 1;  // Used to return weight in grams, kg, etc.

    public:
        HX711();
        virtual ~HX711();

        void begin(uint8_t dout, uint8_t pd_sck, uint8_t gain = 128);
        bool is_ready();
        void wait_ready(unsigned long delay_ms = 0);
        void set_gain(uint8_t gain = 128);
        long read();
        long read_average(uint8_t times = 10);
        double get_value(uint8_t times = 1);
        float get_units(uint8_t times = 1);
        void tare(uint8_t times = 10);
        void set_scale(float scale = 1.f);
        float get_scale();
        void set_offset(long offset = 0);
        long get_offset();
        void power_down();
        void power_up();
};

#endif /* HX711_h */
