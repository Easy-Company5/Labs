#include "drivers/LabsRev2/Libraries.h"

using namespace std;

void Level(Accelerometer accel, LedArray led, uint32_t x_level_colour, uint32_t y_level_colour)
{
    int16_t x_data{}, y_data{}, z_data{};
    float x_accel{}, y_accel{}, z_accel{};

    accel.read_accel_data(&x_data, &y_data, &z_data);

    x_accel = accel.convert_data(x_data);
    y_accel = accel.convert_data(y_data);
    z_accel = accel.convert_data(z_data);

    cout << "x: " << x_accel << ", y: " << y_accel << ", z: " << z_accel << "\n";

    uint8_t led_scaling_factor = abs(x_accel) * 255;
    uint32_t low_colour{(static_cast<uint32_t>(led_scaling_factor) << 24)};
    uint32_t high_colour{(static_cast<uint32_t>(led_scaling_factor) << 16)};
    led.set_all(LED_OFF);

    if (x_accel > 0 & x_accel < 0.1)
    {
        led.set(6, x_level_colour);
    }
    else if (x_accel > 0)
    {
        led.set(7, x_level_colour);
    }
    else if ((x_accel < 0) & (x_accel > -0.1))
    {
        led.set(5, x_level_colour);
    }
    else if (x_accel < 0)
    {
        led.set(4, x_level_colour);
    }
    else
    {
        led.set_multiple(5, x_level_colour, 6, x_level_colour);
    }

    if (y_accel > 0 & y_accel < 0.1)
    {
        led.set(2, y_level_colour);
    }
    else if (y_accel > 0)
    {
        led.set(3, y_level_colour);
    }
    else if ((y_accel < 0) & (y_accel > -0.1))
    {
        led.set(1, y_level_colour);
    }
    else if (y_accel < 0)
    {
        led.set(0, y_level_colour);
    }
    else
    {
        led.set_multiple(1, y_level_colour, 2, y_level_colour);
    }
    led.update();
    sleep_ms(200);
}
