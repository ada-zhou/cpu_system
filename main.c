#include "keyboard.h"
#include "console.h"
#include "shell.h"
#include "interrupts.h"
#include "timer.h"

#define NROWS 20
#define NCOLS 40

void main(void) {
  keyboard_init();
  console_init(NROWS, NCOLS);
  shell_init(1);
  delay(1);
  system_enable_interrupts();
    
  shell_run();
}
