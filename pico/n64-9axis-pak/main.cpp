#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
//
#include "hardware.h"
#include "io.hpp"

// Perhaps belongs in io.cpp?
void gpio_init(){
    gpio_init(CE);
    gpio_init(OE);
    gpio_init(WE);
    gpio_set_dir(CE, GPIO_IN);
    gpio_set_dir(OE, GPIO_IN);
    gpio_set_dir(WE, GPIO_IN);
}

int main(){
    // Enable printing over serial
    stdio_init_all();
    // Set up hardware in initial state
    gpio_init();

    // Set up IRQs
    gpio_set_irq_enabled_with_callback(WE, GPIO_IRQ_LEVEL_LOW, true, &on_joybus_write_to_byte);


    // BLINK!
    // initialise GPIO (Green LED connected to pin 25)
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    int tick = 0;
    //Main Loop 
    while(true){
        printf("%d\n", tick++);
        gpio_put(LED, 1); // Set pin 25 to high
        sleep_ms(1000); // 0.5s delay
        gpio_put(LED, 0); // Set pin 25 to low
        sleep_ms(500); // 0.5s delay

        temp_debug();
    }
}
