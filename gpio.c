#include "gpio.h"

#define FSEL0 (unsigned int *)0x20200000
#define SET0  (unsigned int *) 0x2020001C
#define CLR0  (unsigned int *) 0x20200028
#define LEVEL0 (unsigned int *) 0x20200034

/*
 * initializes gpio - does not need to anything
 */
void gpio_init(void) {
}

/*
 * Set GPIO pin number `pin` to the GPIO function `function`. Does not affect
 * other pins.
 *
 * @param pin the pin number to initialize
 * @param function the GPIO function to set for the pin
 */
void gpio_set_function(unsigned int pin, unsigned int function) {
    
    //Checks if the parameters are in bounds
    if (pin > 53) {
        return;
    }
    if (function > 7) {
        return;
    }
    
    int tens = pin/10;
    int ones = pin%10;
    unsigned int *gfunc = FSEL0 + tens;
    unsigned int curr = *gfunc;
    unsigned int mask = 0b111 << (3*ones);
    
    unsigned int fpin = function << (3*ones);
    
    //clears then puts function in
    mask = ~mask;
    curr &= mask;
    
    curr |= fpin;
    *gfunc = curr;
}

void gpio_set_input (unsigned int pin) {
    gpio_set_function(pin, 0);
}

void gpio_set_output (unsigned int pin ) {
    gpio_set_function(pin, 1);
}

/*
 * Get the function for GPIO pin number `pin`. Should not affect
 * any registers.
 *
 * @param pin the pin number to initialize
 */
unsigned int gpio_get_function(unsigned int pin) {
    if (pin > 53) {
        return 0;
    }
    int tens = pin/10;
    int ones = pin%10;
    unsigned int *gfunc = FSEL0 + tens;
    unsigned int curr = *gfunc;
    unsigned int compare = 0b111 << (3*ones);
    curr &= compare;
    curr >>= (3*ones);
    return curr;
}

/*
 * Sets GPIO pin number `pin` to the value `value`. The GPIO pin should be set
 * the output mode for this call to function correctly.
 *
 * @param pin the pin number to set or clear
 * @param value 1 if the pin should be set, 0 otherwise
 */
void gpio_write(unsigned int pin, unsigned int value) {
    if (pin > 53) {
        return;
    }
    if (value != 0 && value != 1){
        return;
    }
    
    unsigned int *output;
    if (value == 1) {
        output = SET0;
    } else {
        output = CLR0;
    }
    
    if (pin > 31) {
        pin -= 32;
        output += 1;
    }
    
    unsigned int curr = *output;
    unsigned int new = 1 << pin;
    
    curr = new;
    *output = curr;
    
}

/*
 * Read GPIO pin number `pin`. 
 *
 * @param pin the pin number to be read
 */
unsigned int gpio_read(unsigned int pin) {
    if (pin > 53) {
        return 0;
    }
    
    unsigned int *levReg = LEVEL0;
    if (pin > 31) {
        pin -= 32;
        levReg+=1;
    }
    unsigned int curr = *levReg;
    int check = 1 << pin;
    curr &= check;
    curr >>= pin;
    return curr;
    
  
}
