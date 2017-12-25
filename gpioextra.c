#include "memory.h"
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"

#define GPIO_BASE (BCM_BASE+0x200000)

#define GPIO_PUD                       (GPIO_BASE + 0x94)
#define GPPUDCLK0                      (GPIO_BASE + 0x98)
#define GPPUDCLK1                      (GPIO_BASE + 0x9C)

/*
  pud
    00 = Off – disable pull-up/down
    01 = Enable Pull Down control
    10 = Enable Pull Up control
    11 = Reserved
*/
void gpio_set_pud(unsigned pin, unsigned pud) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }
    volatile unsigned* reg_pud = (volatile unsigned*)GPIO_PUD;
    *reg_pud = pud & 3;
    delay_us(5) ;

    volatile unsigned* reg_clk = (volatile unsigned*)GPPUDCLK0;
    *reg_clk = 1 << (pin & 31);
    delay_us(5) ;

    *reg_pud = 0;    
    delay_us(5) ;

    *reg_clk = 0;
    delay_us(5) ;
}

void gpio_set_pullup(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    gpio_set_pud(pin, GPIO_PUD_PULLUP);
}

void gpio_set_pulldown(unsigned pin) {
    gpio_set_pud(pin, GPIO_PUD_PULLDOWN);
}

