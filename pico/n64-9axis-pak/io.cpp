#include <stdio.h>
#include "io.hpp"
#include "hardware.h"
#include "pico/stdlib.h"


// 8 pins / bits
const byte data_pins[] {
  DATA0, DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7
  };
// 16 pins / bits
const byte address_pins[] {
  ADDR00, ADDR01, ADDR02, ADDR03, ADDR04, ADDR05, ADDR06, ADDR07,
  //ADDR08, ADDR09, ADDR10, ADDR11, ADDR12
  };
//
char joybus_buffer[JOYBUS_BUFFER_SIZE];
int joybus_index = 0;
bool is_joybus_buffer_full = false;



// Needed?
int safe_gpio_get(uint gpio){
    int value = gpio_get(gpio) > 0 ? 1 : 0;
    return value;
}

void temp_debug(){
    // DISPLAY / DEBUG BUFFER
    if (is_joybus_buffer_full){
        for (int i = 0; i < JOYBUS_BUFFER_SIZE; i++)
            printf("byte[%d]: %d, ", i, joybus_buffer[i]);
        printf("\n");
        joybus_index = 0;
        is_joybus_buffer_full = false;
    }
}






/// @brief Read a byte from the joybus buffer
void on_joybus_write_to_byte(uint gpio, uint32_t events)
{
    if (joybus_index >= JOYBUS_BUFFER_SIZE)
        return;

    //joybus_buffer[joybus_index] = read_data_8bits();
    joybus_buffer[joybus_index] = joybus_index;
    joybus_index++;

    if (joybus_index >= JOYBUS_BUFFER_SIZE)
        is_joybus_buffer_full = true;
}

/// @brief Set GPIO pins for data (D0-D7) to gpio_mode 
/// @param gpio_mode Either GPIO_IN or GPIO_OUT
void set_data_pins_mode(int gpio_mode)
{
  // Set data pins as output
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++){
    gpio_set_dir(data_pins[i], gpio_mode);
  }
}

void write_data_8bits(byte value)
{
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      bool is_bit_set = (value >> i) & 1;
      gpio_put(data_pins[i], is_bit_set);
  }
}

byte read_data_8bits()
{
  byte data = 0;
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      int bit = safe_gpio_get(data_pins[i]);
      data += bit << i;
  }
  return data;
}

unsigned short read_address_16bits()
{
  unsigned short address = 0;
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      int bit = safe_gpio_get(address_pins[i]);
      address += bit << i;
  }
  return address;
}

unsigned short get_joybus_command_address(unsigned short address)
{
    // Upper 11 bits make the address, so pull it down 5 bits (16 -> 11)
  unsigned short address_11bits = (address >> 5) & 0b0000011111111111;
  return address_11bits;
}

byte get_joybus_command_checksum(unsigned short address)
{
    // Lower 5 bits make the checksum
    unsigned short checksum_5bits = address & 0b0000000000011111;
    return (byte)checksum_5bits;
}
