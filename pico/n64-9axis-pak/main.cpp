#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
//
#include "hardware_config.h"
#include "io.hpp"
#include "state.h"

// State machine state
enum state state = await_write;

// Perhaps belongs in io.cpp?
void gpio_init(){
    // Set up single pins
    gpio_init(CE);
    gpio_init(OE);
    gpio_init(WE);
    gpio_set_dir(CE, GPIO_IN);
    gpio_set_dir(OE, GPIO_IN);
    gpio_set_dir(WE, GPIO_IN);
    // Set up address and data lines
    set_address_pins_mode(GPIO_IN);
    set_data_pins_mode(GPIO_IN);
}


void on_joybus_controller_write_byte(uint gpio, uint32_t events)
{
    // Read byte from 32 byte buffer.
    // Response is true if 32/32 bytes read, false otherwise.
    bool is_buffer_full = joybus_read_byte();
    // Set FSM state accordingly
    state = is_buffer_full ? complete_read : await_write;
}


int main(){
    // Enable printing over serial
    stdio_init_all();
    // Set up hardware in initial state
    gpio_init();

    // Set up IRQs
    gpio_set_irq_enabled_with_callback(WE, GPIO_IRQ_LEVEL_LOW, true, &on_joybus_controller_write_byte);


    // BLINK!
    // initialise GPIO (Green LED connected to pin 25)
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    int tick = 0;
    //Main Loop 
    while(true){
        printf("%d - ", tick++);
        gpio_put(LED, 1); // Set LED on
        sleep_ms(1000);
        gpio_put(LED, 0); // Set LED off
        sleep_ms(500);

        printf("state: %d - ", state);
        temp_debug();
        //printf("\n");

        switch (state)
        {
        // Await 
        case complete_read:
            reset_joybus_state();
            state = await_write;
            break;
        
        // Nothing, wait for state change
        case await_read:
        default:
            break;
        }
    }
}
