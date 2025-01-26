cmake_minimum_required(VERSION 3.12)
set(TITLE pio_pak) # Project title

# Set up Pico SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

add_executable(${TITLE}
    hardware_config.h
    )

pico_generate_pio_header(${TITLE} ${CMAKE_CURRENT_LIST_DIR}/clocked_input.pio)

target_sources(${TITLE} PRIVATE clocked_input.c)

target_link_libraries(${TITLE} PRIVATE
        pico_stdlib
        hardware_pio
        hardware_spi
        )


pico_add_extra_outputs(${TITLE})
