#include "memory.h"
#include "reboot.h"

void reboot(void) {
    const int PM_RSTC = 0x2010001c;
    const int PM_WDOG = 0x20100024;
    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    volatile int* reg_pm_wdog = (volatile int*)PM_WDOG;
    volatile int* reg_pm_rstc = (volatile int*)PM_RSTC;

    // give output time to flush.
    int i;
    for (i = 0; i < 100000; i++) {
        __asm__("nop");
    }

    // timeout = 1/16th of a second? (whatever)
    *reg_pm_wdog = PM_PASSWORD | 1;
    *reg_pm_rstc = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
    while (1);
}
