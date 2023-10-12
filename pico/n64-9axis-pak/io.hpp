#pragma once

#include "pico/stdlib.h"
#include "hardware.h"

#define byte char
#define HIGH 1
#define LOW 0


// Needed?
int safe_gpio_get(uint gpio);
void temp_debug();

void on_joybus_write_to_byte(uint gpio, uint32_t events);
void set_data_pins_mode(int gpio_mode);
void write_data_8bits(byte value);
byte read_data_8bits();
unsigned short read_address_16bits();
unsigned short get_joybus_command_address(unsigned short address);
byte get_joybus_command_checksum(unsigned short address);
