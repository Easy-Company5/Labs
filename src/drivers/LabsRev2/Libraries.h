//Actual Libraries
#include <stdio.h>
#include <array>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include <iostream>
#include "WS2812.pio.h"
#include "arm_math.h"
#include <cmath>
#include <vector>

//Drivers
#include "drivers/LabsRev2/logging/logging.h"
#include "drivers/LabsRev2/WS2812/WS8212.h"
#include "drivers/LabsRev2/Lab9/LIS3DH.h"
#include "drivers/LabsRev2/Lab10/microphone.h"
#include "drivers/LabsRev2/ColourList.h"
#include "drivers/LabsRev2/Init.h"
#include "drivers/LabsRev2/GPIOPins.h"

//Labs
#include "drivers/LabsRev2/Lab7/Lab7.h"
#include "drivers/LabsRev2/Lab9/Lab9.h"
#include "drivers/LabsRev2/Lab10/Lab10.h"