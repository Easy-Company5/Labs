#include "drivers/LabsRev2/Libraries.h"


using namespace std;

volatile int current_task{2}; // Global variable to track the current task

// Flag to track first entry into each task
bool first_entry{true}; // Will be reset when switching cases

void gpio_callback(uint gpio, uint32_t events)
{
    if (gpio == SW1)
    {
        current_task = (current_task + 1) % NUMBER_OF_TASKS;
        first_entry = true; // Set first_entry to true when switching to a new task
    }
}

int main() {
    Initialise();
    uint8_t current_state{1};
// Infinite loop to handle case switching based on the switch state
    Accelerometer accel;
    LedArray led(NUM_LEDS);
    Microphone microphone;


    microphone.init();
    const uint32_t YLevelColour{LED_GREEN};
    const uint32_t XLevelColour{LED_PINK};

    // Set up an interrupt on the SWITCH_PIN for a falling edge (button press)
    gpio_set_irq_enabled_with_callback(SW1, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while (true) {
        switch (current_task)
        {
            case 0: //Lab 7 LED
            {
                LightLED(led, LED_GREEN);
            }

            case 1: //Lab 8 LIS3DH
            {
                Level(accel, led, XLevelColour, YLevelColour);
            }

            case 2: //Lab 9 Microphone
            {
                audio_visualiser(microphone, led);
            }
            
            case 4: //Reset Flags
            {
                first_entry = true;
                std::cout << "Entry flag reset." << std::endl;
                break;
            }

            default:
            {
                if (first_entry)
                {
                    sleep_ms(500);
                    first_entry = false;
                }
                std::cout << "Invalid task." << std::endl;
                break;
            }
            

        }
    }

    return 0;
}
