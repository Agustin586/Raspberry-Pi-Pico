# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/aguat/Documents/Pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(HMI_Project C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(HMI_Project HMI_Project.c )

pico_set_program_name(HMI_Project "HMI_Project")
pico_set_program_version(HMI_Project "0.1")

pico_enable_stdio_uart(HMI_Project 0)
pico_enable_stdio_usb(HMI_Project 1)

# Add the standard library to the build
target_link_libraries(HMI_Project pico_stdlib)

# Add any user requested libraries
target_link_libraries(HMI_Project
        hardware_i2c
        hardware_timer
        )

pico_add_extra_outputs(HMI_Project)

