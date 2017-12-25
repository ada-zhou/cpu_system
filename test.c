#include "keyboard.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
#include "gpio.h"
#include "circular.h"
#include "interrupts.h"
#include "assert.h"

/* 
 * This simple program tests the behavior of the asisgn5 keyboard
 * implementation versus the new-improved assign7 version. If using the
 * assign5 implementation, a key press that arrives while the main program
 * is waiting in delay is simply dropped. Once you upgrade your
 * keyboard implementation to be interrupt-driven, those keys should 
 * be queued up and can be read after delay finishes.
 */


void main(void) 
{
    /*
    // TESTS FOR CIRCULAR BUFFER with capacity 5
    cir_t *cirbuf = cir_new();
    
    assert(cir_empty(cirbuf)==1);
    assert(cir_full(cirbuf)==0);
    
    cir_enqueue(cirbuf, 1);
    cir_enqueue(cirbuf, 2);
    
    assert(cir_empty(cirbuf)==0);
    assert(cir_full(cirbuf)==0);
    
    cir_enqueue(cirbuf, 3);
    
    printf("first: %d ", cir_dequeue(cirbuf));
    
    cir_enqueue(cirbuf, 4);
    
    
    cir_enqueue(cirbuf, 5);
     //cir_enqueue(cirbuf, 6);
    assert(cir_full(cirbuf)==1);
    
    printf("second: %d\n", cir_dequeue(cirbuf));
    printf("third: %d\n", cir_dequeue(cirbuf));
    printf("fourth: %d\n", cir_dequeue(cirbuf));
    printf("fifth: %d\n", cir_dequeue(cirbuf));
    
    assert(cir_empty(cirbuf)==1);
    //cir_enqueue(cirbuf, 6);
    */
   
    keyboard_init();
    printf_init();
    system_enable_interrupts();
    
    while (1) {
        printf("Test program waiting for you to press a key (q to quit): ");
        uart_flush();
        char ch = keyboard_read_char();
        printf("\nRead: %c\n", ch);
        
        if (ch == 'q') break;
        printf("Test program will now pause for 1 second... ");
        uart_flush();
        
        delay(1);
        printf("done.\n");
     
    }

    printf("\nGoodbye!\n");
    
}
