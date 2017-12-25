#include "memory.h"
#include "gpio.h"
#include "gpioevent.h"

#define GPIO_EVENT_DETECT_STATUS       (GPIO_BASE + 0x40)
#define GPIO_RISING_EDGE_ENABLE        (GPIO_BASE + 0x4C)
#define GPIO_FALLING_EDGE_ENABLE       (GPIO_BASE + 0x58)
#define GPIO_HIGH_LEVEL_ENABLE         (GPIO_BASE + 0x64)
#define GPIO_LOW_LEVEL_ENABLE          (GPIO_BASE + 0x70)
#define GPIO_ASYNC_RISING_EDGE_ENABLE  (GPIO_BASE + 0x7C)
#define GPIO_ASYNC_FALLING_EDGE_ENABLE (GPIO_BASE + 0x88)

void gpio_detect_falling_edge(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_FALLING_EDGE_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_rising_edge(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_RISING_EDGE_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_async_rising_edge(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_ASYNC_RISING_EDGE_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_async_falling_edge(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_ASYNC_FALLING_EDGE_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_high_level(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_HIGH_LEVEL_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_low_level(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_HIGH_LEVEL_ENABLE + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    val |= 1 << shift;
    *reg = val;
}

void gpio_detect_event(unsigned pin, unsigned event) {
    switch (event) {
        case GPIO_ASYNC_RISING_EDGE:
            gpio_detect_async_rising_edge(pin);
            break;
        case GPIO_ASYNC_FALLING_EDGE:
            gpio_detect_async_falling_edge(pin);
            break;
        case GPIO_RISING_EDGE:
            gpio_detect_rising_edge(pin);
            break;
        case GPIO_FALLING_EDGE:
            gpio_detect_falling_edge(pin);
            break;
        case GPIO_HIGH_LEVEL:
            gpio_detect_high_level(pin);
            break;
        case GPIO_LOW_LEVEL:
            gpio_detect_low_level(pin);
            break;
        default:
            break;
    }
}


unsigned gpio_check_event(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return 0;
    }

    unsigned addr = GPIO_EVENT_DETECT_STATUS + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    unsigned val = *reg;
    return (val & (1 << shift)) ? 1 : 0;
}

void gpio_clear_event(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return;
    }

    unsigned addr = GPIO_EVENT_DETECT_STATUS + 4 * (pin / 32);
    volatile unsigned* reg = (volatile unsigned*)addr;
    unsigned shift = pin % 32;

    *reg = 1 << shift;
}

unsigned gpio_check_and_clear_event(unsigned pin) {
    if (pin < 0 || pin >= GPIO_MAX) {
        return 0;
    }

    unsigned event = gpio_check_event(pin);
    if (event) {
        gpio_clear_event(pin);
    }

    return event;
}
