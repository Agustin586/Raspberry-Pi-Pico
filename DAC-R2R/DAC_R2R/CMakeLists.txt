# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/aguat/OneDrive/Documentos/Pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(DAC_R2R C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(DAC_R2R DAC_R2R.c SenTable.h TriTable.h)

pico_set_program_name(DAC_R2R "DAC_R2R")
pico_set_program_version(DAC_R2R "0.1")

pico_enable_stdio_uart(DAC_R2R 1)
pico_enable_stdio_usb(DAC_R2R 0)

# Add the standard library to the build
target_link_libraries(DAC_R2R pico_stdlib)

pico_add_extra_outputs(DAC_R2R)

