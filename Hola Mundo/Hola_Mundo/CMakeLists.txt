cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico-projects)

pico_sdk_init()

add_executable(Hola_Mundo Hola_Mundo.c)

target_link_libraries(Hola_Mundo pico_stdlib)

pico_add_extra_outputs(Hola_Mundo)
