cmake_minimum_required(VERSION 3.13)

# Detect if the active kit is an ARM cross-compiler
if(CMAKE_CXX_COMPILER MATCHES "arm-none-eabi")
    message(STATUS "Detected that the current kit is a cross-compiler.")
    set(CrossCompiling 1)
else()
    message(STATUS "Detected that the current kit is a host compiler. Building the test harness.")
endif()

# Detect if the active kit is an ARM cross-compiler
if(CrossCompiling)
    # Yes, build for the RP2040

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    # Specify an increased oscillator startup delay to work around a potential hardware issue where the 
    # clock is slow to boot.
    add_compile_definitions(PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

    # Pull in Raspberry Pi Pico SDK (must be before project)
    include(pico_sdk_import.cmake)

    project(cc3501-labs C CXX ASM)
    
    # Initialise the Raspberry Pi Pico SDK
    pico_sdk_init()
    # You could also use ARM_ALL_FAST_TABLES if you need all fast tables

    # Set up CMSIS-DSP
    set(CMSISCORE "${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS_5/CMSIS/Core")
    # set options for which parts of CMSIS-DSP are to be included
    set(BASICMATH ON)
    set(COMPLEXMATH ON)
    set(CONTROLLER OFF)
    set(FASTMATH ON)
    set(FILTERING OFF)
    set(MATRIX OFF)
    set(STATISTICS OFF)
    set(SUPPORT OFF)
    set(TRANSFORM ON)
    set(SVM OFF)
    set(BAYES OFF)
    set(DISTANCE OFF)
    set(INTERPOLATION OFF)
    set(QUATERNIONMATH OFF)
    set(CONFIGTABLE ON)
    set(RFFT_Q15_1024 ON) # which FFT constants are hard-coded into the app
    add_subdirectory(lib/CMSIS-DSP/Source bin_dsp)

    add_executable(labs)
    target_sources(labs 
    ## REFERENCE THE FILE YOU WANNA BUILD
        PUBLIC
        src/main.cpp
        src/drivers/LabsRev2/logging/logging.cpp
        # src/drivers/LABS/LED.cpp
        # src/drivers/LABS/Misc.cpp
        # src/drivers/LABS/microphone.cpp
        # src/drivers/LABS/Switch.cpp
        # src/drivers/LABS/Init.cpp
        # src/drivers/LABS/Assignment/UartTransmit.cpp
        # src/drivers/LABS/Assignment/LIS3DHS.cpp
        # src/drivers/LABS/Assignment/HCSR04.cpp
        # src/drivers/LABS/Assignment/A4988.cpp
        # src/drivers/LABS/Assignment/LEDs.cpp
        # src/drivers/LABS/LIS3DH.cpp
        # src/drivers/LABS/microphone.cpp
        src/drivers/LabsRev2/Lab7/Lab7.cpp
        src/drivers/LabsRev2/Lab10/microphone.cpp
        # src/drivers/LABS/SwitchRev2.cpp
        src/drivers/LabsRev2/WS2812/WS2812.cpp
        src/drivers/LabsRev2/Lab9/LIS3DH.cpp
        src/drivers/LabsRev2/Lab9/Lab9.cpp
        src/drivers/LabsRev2/Lab10/Lab10.cpp
        src/drivers/LabsRev2/Init.cpp

    )
    target_include_directories(labs
        PUBLIC 
        src/
    )

    pico_set_program_name(labs "cc3501-labs")
    pico_set_program_version(labs "0.2")

    # select UART for standard IO
    pico_enable_stdio_uart(labs 1)
    pico_enable_stdio_usb(labs 0)

    # compile the PIO file
    pico_generate_pio_header(labs ${CMAKE_CURRENT_LIST_DIR}/src/drivers/WS2812/WS2812.pio)

    # Add the standard library to the build
    target_link_libraries(labs
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_pwm
        hardware_adc
        CMSISDSP
        pico_rand
    )

    pico_add_extra_outputs(labs)

else()

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 20)

    # We are building natively, so create the test harness instead
    project(cc3501-labs CXX)

    add_executable(labs)
    target_sources(labs 
    ## REFERENCE THE FILE YOU WANNA BUILD
        PUBLIC
        src/main.cpp
        src/drivers/LabsRev2/logging/logging.cpp
        # src/drivers/LABS/LED.cpp
        # src/drivers/LABS/Misc.cpp
        # tests/mocks/pico/stdlib.cpp
        # tests/mocks/pico/time.cpp
        # tests/mocks/hardware/gpio.cpp
        # tests/mocks/hardware/pio.cpp
        # tests/mocks/ws2812.cpp
        # src/drivers/LABS/microphone.cpp
        # src/drivers/LABS/Switch.cpp
        # src/drivers/LABS/Init.cpp
        # src/drivers/LABS/Assignment/UartTransmit.cpp
        # src/drivers/LABS/Assignment/UartTransmit.cpp
        # src/drivers/LABS/Assignment/LIS3DHS.cpp
        # src/drivers/LABS/Assignment/HCSR04.cpp
        # src/drivers/LABS/Assignment/A4988.cpp
        # src/drivers/LABS/Assignment/LEDs.cpp
        # src/drivers/LABS/LIS3DH.cpp
        # src/drivers/LABS/microphone.cpp
        src/drivers/LabsRev2/Lab7/Lab7.cpp
        src/drivers/LabsRev20/Lab10/microphone.cpp
        # src/drivers/LABS/SwitchRev2.cpp
        src/drivers/LabsRev2/WS2812/WS2812.cpp
        src/drivers/LabsRev2/Lab9/LIS3DH.cpp
        src/drivers/LabsRev2/Lab9/Lab9.cpp
        src/drivers/LabsRev2/Lab10/Lab10.cpp

        src/drivers/LabsRev2/Init.cpp

    )
    target_include_directories(labs
        PUBLIC 
        src/
        tests/
        tests/mocks/
    )
    target_compile_definitions(labs 
        PUBLIC
        TEST_HARNESS=1
    )

endif()

target_compile_definitions(labs 
    PUBLIC
    LOG_DRIVER_STYLE=${LogDriverImplementation}
)