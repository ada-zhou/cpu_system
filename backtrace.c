#include "backtrace.h"
#include "printf.h"

int backtrace(struct frame f[], int max_frames)
{
    unsigned int *curr_fpvalue;
    __asm__("mov %0, fp" : "=r" (curr_fpvalue));
 
    
    int count = 0;
    unsigned int  *savedfp = curr_fpvalue - 3;
    
    while ((count < max_frames) && *savedfp) {
        f[count].fn_resume = *(curr_fpvalue-1);
        f[count].fn_first = (*((unsigned int*)(*savedfp))) - 12;
        count++;
        curr_fpvalue = (unsigned int *)*savedfp;
        savedfp = curr_fpvalue - 3;
    }
    
    return count;
}


void print_frames(struct frame f[], int n)
{
    
    unsigned int * end;
    
    for (int i = 0; i < n; i++) {
        char *name = "???";
        end = (unsigned int *)f[i].fn_first - 1;
        
        if ((*end & (0xff) << (6*4)) == ((0xff) << (6*4))){
            int length = *end & ~0xff000000;
            
            char *begin = (char *)end - length;
            name = begin;
        }
    
        int delta = f[i].fn_resume - f[i].fn_first;

        // Don't change this print format!
        printf("#%d 0x%x at %s+%d\n", i, f[i].fn_resume, name, delta);
    }
}

void print_backtrace(void)
{
    struct frame f[11];
    
    int count = backtrace(f, 11);
    struct frame final[10];
    for (int i = 0; i < 10; i++) {
        final[i] = f[i+1];
    }
    print_frames(final, count);
}
