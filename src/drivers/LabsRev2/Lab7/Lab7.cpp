#include "drivers/LabsRev2/Libraries.h"


void LightLED(LedArray led, uint32_t led_colour)
{
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        for (size_t j = 0; j < NUM_LEDS - i; j++)
        {
            led.set(j, led_colour);
            if (j > 0)
            {
                led.set(j - 1, LED_OFF);
            }
            led.update();
            sleep_ms(50);
        }
        busy_wait_ms(50);
    }

    for (size_t i = (NUM_LEDS + 1); i > 0; i--)
    {
        for (size_t j = NUM_LEDS; j > (NUM_LEDS - i); j--)
        {
            led.set((j - 1), LED_OFF);
            if (j < NUM_LEDS)
            {
                led.set(j, led_colour);
            }
            led.update();
            sleep_ms(50);
        }
    }
}
