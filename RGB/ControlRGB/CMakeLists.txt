# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/aguat/Documents/Pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ControlRGB C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ControlRGB ControlRGB.c )

pico_set_program_name(ControlRGB "ControlRGB")
pico_set_program_version(ControlRGB "0.1")

pico_enable_stdio_uart(ControlRGB 1)
pico_enable_stdio_usb(ControlRGB 0)

# Add the standard library to the build
target_link_libraries(ControlRGB pico_stdlib hardware_pwm)

# Add any user requested libraries
target_link_libraries(ControlRGB
        hardware_i2c
        hardware_pwm
        )

pico_add_extra_outputs(ControlRGB)

