#ifndef LEDS_H
#define LEDS_H

#define NUM_LEDS 8  // Number of LEDs in the strip

void LED_PIO_init();
void LEDs_init(uint32_t *led_data, int i, uint8_t red, uint8_t green, uint8_t blue);
void LEDs_update(uint32_t *led_data);
void LEDs_off(uint32_t *led_data);
void LEDs_blue(uint32_t *led_data);
void LEDs_green(uint32_t *led_data);
void LEDs_red(uint32_t *led_data);

#endif
