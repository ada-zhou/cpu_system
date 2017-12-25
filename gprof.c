#include "gprof.h"
#include "armtimer.h"
#include "malloc.h"
#include "console.h"

/* Interval for profiler timer interrupts */
#define GPROF_TIMER_INTERVAL 0x10

void gprof_init(void) { 
    /* FIXME: Set up timer interrupts and initialize your address counts */
}

void gprof_on(void) {
    /* FIXME: Turn on profiling and reset counts */
}

void gprof_off(void) {
    /* FIXME: Turn off profiling */
}

int gprof_is_active(void) {
    /* FIXME: Report whether profiling on */
    return 0;
}

void gprof_dump(void) {
    console_printf("******************\n");
    console_printf(" PC    |     COUNT\n");
    
    /* FIXME: cycle through all addresses in text segment 
       and print out their count using:
      
       console_printf("%x:\t%d count\n", address, cnt); 
    */
}

void gprof_vector(unsigned pc) {
  /* FIXME: Increment the count for the given address */
}
