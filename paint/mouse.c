#include "gpio.h"
#include "gpioextra.h"
#include "gpioevent.h"
#include "circular.h"
#include "interrupts.h"
#include "mouse.h"

#define MOUSE_CLK GPIO_PIN25
#define MOUSE_DATA GPIO_PIN26

#define CMD_RESET 0xFF
#define CMD_ENABLE_DATA_REPORTING 0xF4

static cir_t *cir;

static void mouse_write(unsigned char data);

void mouse_init() {
  cir = cir_new();

  gpio_set_function(MOUSE_CLK, GPIO_FUNC_INPUT);
  gpio_set_pullup(MOUSE_CLK);
  gpio_set_function(MOUSE_DATA, GPIO_FUNC_INPUT);
  gpio_set_pullup(MOUSE_DATA);

  // FIXME: Initialize mouse.

  // Placeholder because mouse_write is unused otherwise.
  (void) mouse_write;
}

mouse_event_t mouse_read_event() {
  mouse_event_t evt;
  // FIXME: Read scancode(s) and fill in evt.

  return evt;
}

int mouse_read_scancode() {
  // FIXME: Read from circular buffer.
  return 0;
}

static void mouse_write(unsigned char data) {
  // FIXME: Send host->mouse packet.
}

void mouse_vector() {
  // FIXME: Handle an interrupt from mouse clock line.
}
