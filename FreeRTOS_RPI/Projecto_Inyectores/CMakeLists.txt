# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/aguat/OneDrive/Documentos/Pico/pico-sdk")
set(FREERTOS_KERNEL_PATH "C:/Users/aguat/OneDrive/Documentos/GitHub/FreeRTOS-Kernel")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(Projecto_Inyectores C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
                Projecto_Inyectores 
                Projecto_Inyectores.c 
                Mef.h
                Mef.c
                B_Nextion.h
                B_Nextion.c
                B_Control.h
                B_Control.c
                ConfigFreeRtos.h
                ConfigFreeRtos.c
                Defines.h
                Includes.h
                RTC_DS3231.h
                RTC_DS3231.c
                Adc.h
                Adc.c
                )

pico_set_program_name(Projecto_Inyectores "Projecto_Inyectores")
pico_set_program_version(Projecto_Inyectores "0.1")

# Add the standard library to the build
target_link_libraries(
                        Projecto_Inyectores 
                        pico_stdlib
                        FreeRTOS-Kernel
                        FreeRTOS-Kernel-Heap1
                        pico_multicore
                        hardware_i2c
                        hardware_pwm
                        hardware_irq
                        hardware_gpio
                        hardware_uart
                        hardware_adc
                        )

# pico_enable_stdio_usb(Projecto_Inyectores 1)
# pico_enable_stdio_uart(Projecto_Inyectores 0)

target_compile_definitions(Projecto_Inyectores PRIVATE
                        mainRUN_FREE_RTOS_ON_CORE=0
                        PICO_STCK_SIZE=0X1000
                        )

pico_add_extra_outputs(Projecto_Inyectores)

