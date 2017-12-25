#include <stdio.h>
#include "gpio.h"
#include "ps2.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "uart.h"
#include "gpioevent.h"
#include "interrupts.h"
#include "circular.h"

const unsigned CLK  = GPIO_PIN23;
const unsigned DATA = GPIO_PIN24;

static unsigned char mod = 0;
static unsigned int enter = 0;

int cnt = -1;

static char scan = 0;
static int odd = 0;

static cir_t *cirbuf;


void wait_for_falling_clock_edge() {
    while(gpio_read(CLK) == 0) {}
    while(gpio_read(CLK) == 1) {}
}



void keyboard_init(void) {
    gpio_set_function(CLK, GPIO_FUNC_INPUT);
    gpio_set_pullup(CLK);
    
    
    gpio_detect_falling_edge(CLK);
    interrupts_enable(INTERRUPTS_GPIO3);
    
    gpio_set_function(DATA, GPIO_FUNC_INPUT);
    gpio_set_pullup(DATA);
    
    
    cirbuf = cir_new();
    
}

void keyboard_vector (void) {
    
    if (!gpio_check_and_clear_event(CLK)) {
        return;
    }
    
    if (cnt == -1) {
        if (gpio_read(DATA) != 0) {
            return;
        } else {
            cnt++;
        }
    } else if (cnt < 8 && cnt > -1){
        scan += (gpio_read(DATA) << cnt);
        odd += (gpio_read(DATA));
        cnt++;
    } else if (cnt == 8) {
        if (((odd + gpio_read(DATA))%2) != 1) {
            scan = 0;
            cnt = -1;
            odd = 0;
            return;
        } else {
            cnt++;
        }
    } else {
        if(gpio_read(DATA) != 1) {
            scan = 0;
            cnt = -1;
            odd = 0;
            return;
        } else {
            cir_enqueue (cirbuf, scan);
            
            scan = 0;
            cnt = -1;
            odd = 0;
        }
        
    }
}


int keyboard_read_scancode(void) {
    return cir_dequeue(cirbuf);
}

key_event_t keyboard_read_event(void) {
    //ASK ABOUT 0XE0 AND WHAT IT DOES?
    key_event_t event;
    int scan = keyboard_read_scancode();
    if (scan == 0xE0) return event;
    if (scan == 0xF0) {
        int code = keyboard_read_scancode();
        if (code == 0xE0) return event;
        event.code = code;
        event.down = 0;
    } else {
        event.code = scan;
        event.down = 1;
    }
    return event;
}


unsigned char getMod (unsigned char code) {
    unsigned char mod = 0;
    switch (code) {
        case KEY_LEFT_SHIFT:
            mod = KEY_MOD_LEFT_SHIFT;
            break;
        case KEY_RIGHT_SHIFT:
            mod = KEY_MOD_RIGHT_SHIFT;
            break;
        case KEY_LEFT_CONTROL:
            mod = KEY_MOD_LEFT_CTRL;
            break;
        case KEY_LEFT_ALT:
            mod = KEY_MOD_LEFT_ALT;
            break;
        case KEY_CAPS:
            mod = KEY_MOD_CAPS;
            break;
    }
    return mod;
}

key_press_t keyboard_read_press(void) {
    key_press_t press;
    press.modifiers = 0;
    key_event_t event = keyboard_read_event();
    while (1) {
        if (event.down) {
            if (event.code == KEY_CAPS ){
                if ((mod & KEY_MOD_CAPS) == KEY_MOD_CAPS){
                    mod ^= KEY_MOD_CAPS;
                } else {
                    mod |= KEY_MOD_CAPS;
                }
            } else {
                mod |= getMod(event.code);
            }
            if (getMod(event.code) == 0){
                press.modifiers = mod;
                press.code = event.code;
                return press;
            }
            
        } else if (event.code != KEY_CAPS){
            mod ^= getMod(event.code);
        }
        event = keyboard_read_event();
    }
    
    return press;
}

char keyboard_read_char(void) {
    if (enter){
        enter = 0;
        return '\n';
    }
    
    key_press_t press = keyboard_read_press();
    
    if (press.code == KEY_ENTER){
        enter = 1;
        return '\r';
    }
    
    while ((!is_char[press.code]) || ((press.modifiers & KEY_MOD_CTRL) != 0) || ((press.modifiers & KEY_MOD_ALT) != 0)) {
        press = keyboard_read_press();
    }
    
    if (((press.modifiers & KEY_MOD_SHIFT) != 0 )|| ((press.modifiers & KEY_MOD_CAPS) == KEY_MOD_CAPS)){
        return shift_char_from_scan[press.code];
    }
    
    return char_from_scan[press.code];
    
}

