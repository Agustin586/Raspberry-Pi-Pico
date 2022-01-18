cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(main
        main.c
        )

pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../../Common/include)

target_link_libraries(main pico_stdlib hardware_i2c hardware_pwm hardware_irq FreeRTOS-Kernel FreeRTOS-Kernel-Heap1)
pico_add_extra_outputs(main)