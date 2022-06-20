cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# add_library(main INTERFACE)
# target_sources(main INTERFACE
#         main.c)
# target_include_directories(main INTERFACE
#         ${CMAKE_CURRENT_LIST_DIR}
#         )
# target_link_libraries(main INTERFACE
#         FreeRTOS-Kernel
#         FreeRTOS-Kernel-Heap1
#         pico_stdlib
#         pico_multicore
#         hardware_i2c
#         hardware_pwm
#         hardware_irq
#         )

# add_executable(on_core_zero)
# target_link_libraries(on_core_zero on_either_core_common)
# pico_add_extra_outputs(on_core_zero)
# pico_enable_stdio_usb(on_core_zero 1)

# add_executable(on_core_one)
# target_link_libraries(on_core_one main)
# target_compile_definitions(on_core_one PRIVATE
#         mainRUN_FREE_RTOS_ON_CORE=1
#         PICO_STACK_SIZE=0x1000
# )
# pico_add_extra_outputs(on_core_one)
#pico_enable_stdio_usb(on_core_one 1)
add_executable( inyectores 
        main.c
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
        )
target_include_directories(inyectores PRIVATE
${CMAKE_CURRENT_LIST_DIR}
${CMAKE_CURRENT_LIST_DIR}/../../Common/include)
# pico_enable_stdio_uart(inyectores 0)
# pico_enable_stdio_usb(inyectores 1)
target_link_libraries(inyectores
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap1
        pico_stdlib
        pico_multicore
        hardware_i2c
        hardware_pwm
        hardware_irq
        hardware_gpio
        hardware_uart
        )
target_compile_definitions(inyectores PRIVATE
        mainRUN_FREE_RTOS_ON_CORE=0
        PICO_STACK_SIZE=0x1000
)
pico_add_extra_outputs( inyectores )