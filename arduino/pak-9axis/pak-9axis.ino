// Included packages
// Additional board manage URLs: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
// Arduino Pico W poackages... sorry, didn't double check which exactly.

// PICO W GPIO usage
#define DATA0 0
#define DATA1 1
#define DATA2 2
#define DATA3 3
#define DATA4 4
#define DATA5 5
#define DATA6 6
#define DATA7 7
#define ADDR00 8
#define ADDR01 9
#define ADDR02 10
#define ADDR03 11
#define ADDR04 12
#define ADDR05 13
#define ADDR06 14
#define ADDR07 15
#define ADDR08 16
#define ADDR09 17
#define ADDR10 18
#define ADDR11 19
#define I2C0_SDA 20 // I2C for sensor
#define I2C0_SCL 21 // I2C for sensor
#define ADDR12 22
// NO ADDR 13, 14, 15 (0x2000, 0x4000, 0x8000)
#define CE 26
#define OE 27 // OUTPUT ENABLE
#define WE 28 // WRITE ENABLE

#include "state.c"


// 8 pins / bits
const byte data_pins[] {
  DATA0, DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7
  };
// 16 pins / bits
const byte address_pins[] {
  ADDR00, ADDR01, ADDR02, ADDR03, ADDR04, ADDR05, ADDR06, ADDR07,
  //ADDR08, ADDR09, ADDR10, ADDR11, ADDR12
  };

// write buffer from 0x03
byte joybus_write_index = 0;
byte joybus_write_buffer[32]; // is garbage on startup
//
enum state state = await_write;
//bool ce_triggered = false;
//bool we_triggered = false;
long long count = 0;

void setup()
{
  // Set address pins as input
  for (int i = 0; i < sizeof(address_pins) / sizeof(byte); i++)
    pinMode(address_pins[i], INPUT);
  //set_data_pins_mode(OUTPUT);
  pinMode(CE, INPUT_PULLUP);
  pinMode(OE, INPUT);
  pinMode(WE, INPUT);

  // Turn on LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // prepare FSM
  reset_state();

  // Attach some interrupts
  //attachInterrupt(CE, toggle_ce, RISING);
  //attachInterrupt(WE, toggle_we, FALLING);
  attachInterrupt(WE, on_joybus_write, FALLING);

  // Start serial - debug only
  Serial.begin(115200);
  Serial.println("Setup complete.");
}


void loop()
{
  if (joybus_write_index == 32)
    state = complete_write;
  else
    return;

  switch(state)
  {
    case complete_write:
      //Serial.print("Write completed with data: \t");
      Serial.print(count++);
      Serial.print("\t");
      for (int i = 0; i < 32; i++){
        Serial.print(joybus_write_buffer[i]);
        Serial.print(",\t");
      }
      Serial.println();
      reset_state();
      break;

    case await_read:
    case await_write:
    default:
      break;
  }

  // delay(100);
  // Serial.println("hello");
}

// void toggle_ce(){
//   ce_triggered = true;
//   if (ce_triggered & we_triggered){
//     on_joybus_write();
//   }
// }
// void toggle_we(){
//   we_triggered = true;
//   if (we_triggered & we_triggered){
//     on_joybus_write();
//   }
// }

void reset_state()
{
  state = await_write;
  joybus_write_index = 0;
  for (int i = 0; i < 32; i++)
    joybus_write_buffer[i] = 0;
  // expect input  
  set_data_pins_mode(INPUT);
}

void on_joybus_write()
{
  //
  //ce_triggered = false;
  //we_triggered = false;
  //
  joybus_write_buffer[joybus_write_index] = read_data_8bits();
  joybus_write_index++;
}





void set_data_pins_mode(int mode)
{
  // Set data pins as output
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++){
    pinMode(data_pins[i], mode);
  }
}

void write_data_8bits(byte value)
{
  set_data_pins_mode(OUTPUT);
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      bool bit_is_set = (value >> i) & 1;
      digitalWrite(data_pins[i], bit_is_set ? HIGH : LOW);
  }
}

byte read_data_8bits()
{
  byte data = 0;
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      int bit = digitalRead(data_pins[i]);
      data += bit << i;
  }
  return data;
}

unsigned short read_address_16bits()
{
  unsigned short address = 0;
  for (int i = 0; i < sizeof(data_pins) / sizeof(byte); i++)
  {
      int bit = digitalRead(address_pins[i]);
      address += bit << i;
  }
  return address;
}

unsigned short get_address_11bits(unsigned short address_16bits)
{
  unsigned short address_11bits = (address_16bits >> 5) & 0b0000011111111111;
  return address_11bits;
}

byte get_address_checksum(unsigned short address_16bits)
{
  byte checksum = (byte)((address_16bits >> 11) & 0b0000000000011111);
  return checksum;
}
