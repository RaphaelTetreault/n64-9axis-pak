#include <stdio.h>

#include "pico/stdlib.h"

#include "hardware_config.h"
#include "io.hpp"
#include "state.h"


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
int extra_calls = 0;
bool is_joybus_buffer_full = false;



// Needed?
int safe_gpio_get(uint gpio){
    // It only states "non-zero" for high
    int value = gpio_get(gpio) > 0 ? 1 : 0;
    return value;
}

void temp_debug(){
    // DISPLAY / DEBUG BUFFER
    //if (is_joybus_buffer_full){
        for (int i = 0; i < JOYBUS_BUFFER_SIZE; i++)
            printf("%d ", joybus_buffer[i]);
            //printf("[%d]:%d ", i, joybus_buffer[i]);
        printf("\n");
        //joybus_index = 0;
    //}
}




bool joybus_read_byte(){
  // indicate buffer is already full
  if (joybus_index >= JOYBUS_BUFFER_SIZE){
    extra_calls++;
    return true;
  }

  // Read byte from data bus
  joybus_buffer[joybus_index] = read_data_8bits();
  joybus_index++;

  //indicate if buffer is now full
  bool is_joybus_buffer_full = joybus_index >= JOYBUS_BUFFER_SIZE;
  return is_joybus_buffer_full;
}

void reset_joybus_state(){
  printf("joybus index: %d, extra calls: %d\n", joybus_index, extra_calls);
  joybus_index = 0;
  extra_calls = 0;
}


void set_pins_mode(const byte gpio[], byte gpio_array_size, int gpio_mode){
  // Set data pins as output
  for (int i = 0; i < gpio_array_size; i++)
  {
    gpio_set_dir(gpio[i], gpio_mode);
  }
}
void set_data_pins_mode(int gpio_mode){
  int array_size = sizeof(data_pins) / sizeof(byte);
  set_pins_mode(data_pins, array_size, gpio_mode);
}
void set_address_pins_mode(int gpio_mode){
  int array_size = sizeof(address_pins) / sizeof(byte);
  set_pins_mode(address_pins, array_size, gpio_mode);
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
