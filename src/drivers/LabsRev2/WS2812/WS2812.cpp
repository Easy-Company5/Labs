#include "drivers/LabsRev2/Libraries.h"


LedArray::LedArray(int number_of_leds)
    : number_of_leds_{number_of_leds}, led_data_(number_of_leds, 0), last_update_led_data_(number_of_leds, 0)
{
    init();
}

void LedArray::init()
{
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
    update();
}

void LedArray::set(uint8_t position, uint32_t colour)
{
    if (position < number_of_leds_)
    {
        led_data_[position] = colour;
    }
}


void LedArray::set_all(uint32_t colour)
{
    for (size_t i = 0; i < number_of_leds_; i++)
    {
        set(i, colour);
    }
}

uint32_t LedArray::convert_hsv_rgb(uint16_t hue, float saturation, float value)
{
    float c{value * saturation};
    float hue_sector = (hue / 60.0);
    float x = c * (1 - fabs(fmod(hue_sector, 2.0f) - 1));
    float red_float{};
    float green_float{};
    float blue_float{};
    switch ((int)hue_sector)
    {
    case 0:
        red_float = c;
        green_float = x;
        blue_float = 0;
        break;
    case 1:
        red_float = x;
        green_float = c;
        blue_float = 0;
        break;
    case 2:
        red_float = 0;
        green_float = c;
        blue_float = x;
        break;
    case 3:
        red_float = 0;
        green_float = x;
        blue_float = c;
        break;
    case 4:
        red_float = x;
        green_float = 0;
        blue_float = c;
        break;
    case 5:
        red_float = c;
        green_float = 0;
        blue_float = x;
        break;
    default:
        break;
    }
    float m = value - c;
    red_float += m, green_float += m, blue_float += m;
    uint8_t red = red_float * 255;
    uint8_t green = green_float * 255;
    uint8_t blue = blue_float * 255;
    uint32_t rgb_colour = (red << 24) | (green << 16) | (blue << 8);
    return rgb_colour;
}

void LedArray::update()
{
    for (size_t i = 0; i < number_of_leds_; i++)
    {
        // printf("0x%08x\n", led_data_[i]);
        pio_sm_put_blocking(pio0, 0, led_data_[i]);
        // mark LED data as set by setting last bit to 1
        last_update_led_data_[i] = led_data_[i];
        busy_wait_us(200);
    }
}

uint32_t LedArray::status(uint8_t position)
{
    return last_update_led_data_[position];
}

bool LedArray::is_updated(uint8_t position)
{
    return (led_data_[position] == last_update_led_data_[position]);
}

void LedArray::off()
{
    for (size_t i = 0; i < number_of_leds_; i++)
    {
        pio_sm_put_blocking(pio0, 0, 0);
    }
};