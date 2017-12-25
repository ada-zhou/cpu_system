#include "printf.h"
#include "pi.h"

#define GPIO_FSEL3 ((unsigned *)0x2020000c)
#define GPIO_SET1  ((unsigned *)0x20200020)
#define GPIO_CLR1  ((unsigned *)0x2020002c)

#define ABORT_OUTPUT (1 << (3*(PWR%10)))
#define ABORT_BIT    (1 << (PWR-32))

#define DELAY 0x100000

// Flash the red POWER LED
void abort(void) 
{
    *GPIO_FSEL3 = ABORT_OUTPUT;
    while (1) {
        *GPIO_SET1 = ABORT_BIT;
        for (volatile int i = 0; i < DELAY; i++) ;
        *GPIO_CLR1 = ABORT_BIT;
        for (volatile int i = 0; i < DELAY; i++) ;
    }
}

void abort_assert(int truth, char *expr_str, char *file, int line)
{
    if (!truth) {
        printf("File %s, line %d: Assertion '%s' failed.\n", file, line, expr_str);
        abort();
    }
}
