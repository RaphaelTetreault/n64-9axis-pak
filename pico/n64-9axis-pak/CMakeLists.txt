cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

# Define name of main output binary
set(TITLE n64_9axis_pak)

project(${TITLE})

pico_sdk_init()

add_executable(${TITLE}
    main.cpp
    io.cpp io.hpp
    hardware_config.h
    state.h
    )

# Include dependencies
target_link_libraries(${TITLE}
    pico_stdlib
    hardware_i2c
    )
#
pico_enable_stdio_usb(${TITLE} 1)
pico_enable_stdio_uart(${TITLE} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${TITLE})