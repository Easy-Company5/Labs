#include "pico/stdlib.h"
#include "HX711.h"

HX711::HX711() {
}

HX711::~HX711() {
}

void HX711::begin(uint8_t dout, uint8_t pd_sck, uint8_t gain) {
    PD_SCK = pd_sck;
    DOUT = dout;

    // Initialize the GPIO pins for RP2040
    gpio_init(PD_SCK);
    gpio_set_dir(PD_SCK, GPIO_OUT);
    gpio_put(PD_SCK, 0);  // Ensure SCK starts LOW

    gpio_init(DOUT);
    gpio_set_dir(DOUT, GPIO_IN);

    set_gain(gain);
}

bool HX711::is_ready() {
    return gpio_get(DOUT) == 0;
}

void HX711::set_gain(uint8_t gain) {
GAIN = 1;
    }

long HX711::read() {
    wait_ready();

    unsigned long value = 0;
    uint8_t data[3] = {0};
    uint8_t filler = 0x00;

    for (int i = 0; i < 24; i++) {
        gpio_put(PD_SCK, 1);  // Pulse SCK HIGH
        value = value << 1;
        gpio_put(PD_SCK, 0);  // Pulse SCK LOW

        if (gpio_get(DOUT)) {
            value++;
        }
    }

    // Set the channel and the gain factor for the next reading using the clock pin.
    for (unsigned int i = 0; i < GAIN; i++) {
        gpio_put(PD_SCK, 1);
        sleep_us(1);  // 1 microsecond delay
        gpio_put(PD_SCK, 0);
    }

    // Replicate the most significant bit to pad out a 32-bit signed integer
    if (data[2] & 0x80) {
        filler = 0xFF;
    } else {
        filler = 0x00;
    }

    value = ((unsigned long)filler << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    return (long)value;
}

void HX711::wait_ready(unsigned long delay_ms) {
    while (!is_ready()) {
        sleep_ms(delay_ms);  // Use RP2040's sleep function
    }
}

long HX711::read_average(uint8_t times) {
    long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}

double HX711::get_value(uint8_t times) {
    return read_average(times) - OFFSET;
}

float HX711::get_units(uint8_t times) {
    return get_value(times) / SCALE;
}

void HX711::tare(uint8_t times) {
    double sum = read_average(times);
    set_offset(sum);
}

void HX711::set_scale(float scale) {
    SCALE = scale;
}

float HX711::get_scale() {
    return SCALE;
}

void HX711::set_offset(long offset) {
    OFFSET = offset;
}

long HX711::get_offset() {
    return OFFSET;
}

void HX711::power_down() {
    gpio_put(PD_SCK, 1);
    sleep_ms(1);
}

void HX711::power_up() {
    gpio_put(PD_SCK, 0);
}
