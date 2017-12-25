#include <stdarg.h>
#include "printf.h"
#include "uart.h"
#include "gpio.h"

void printf_init() {
  uart_init();
}

int unsigned_to_base(char *buf, int n, unsigned int val, int base, int width) {

    
    unsigned int mBase = 1;

    int count = 0; //keeps track of where in buf we are
    int length = 1; //keeps track of how long the val is in the base
    int charRep = 0;
    
    unsigned int v = val;
    
    while (v != 1){
        v = v/base;
        if (v == 0){
            break;
        }
        mBase*=base;
        length++;
    }
    
    
    //checks if width is longer than the length, if so then find the zeroes needed to added to satisfy width
    int zero = 0;
    if (width > length) {
        zero = width - length;
    }

    //create array the size needed to store the value
    char temp [zero + length];
    
    //adds in 0's to the beginning of array as needed
    for (int i = 0; i < zero; i++) {
        temp[i] = '0';
    }
    
    
    
    //adds the values needed to the temp array to represent the val
    for (int i = 0; i < length; i++){
        charRep = val/mBase;
        val = val%mBase;
        
        if (charRep > 9) {
            charRep = charRep - 10 + 'a';
        } else {
            charRep = charRep + '0';
        }
        
        temp[i+zero] = (char) charRep;
        
        mBase = mBase/base;
    }
    
    int itemp = 0;
    //copies the values of the temp array over to buf until buf ends or temp ends
    while ((count < (n - 1)) && (itemp < (zero+length))) {

        *buf = temp[itemp];
        buf++;
        count++;
        itemp++;
    }
    
    if (n > 0) {
        *buf = '\0';
    }
    
    
    
  return zero+length;
}

int signed_to_base(char *buf, int n, int val, int base, int width) {
    int result = 0;
    
    //checks if value is negative
    if (val < 0 && base == 10) {
        //if neg then check if there is space in buf for negative sign
        
        if (n > 1) {
            *buf = '-';
            buf++;
            n--;
            width--;
            
        }
            val = val*(-1);
            result = unsigned_to_base(buf, n, val, base, width) + 1;
        
    } else {
        result = unsigned_to_base(buf, n, val, base, width);
    }
    
  return result;
}



int vsnprintf(char *s, int n, const char *format, va_list args) {    
    int count = 0; //keeps track of total number added to s
    
    int added = 0;
    
    //increments through format until it reaches the end or there is no longer available string space
    while (*format != '\0') {
        //checks if there is a format string (started by '%')
        if (*format == '%') {
            int width = 0;
            
            int base = 0; //stores base if user will be formating a num (decimal/hex/binary)
            
            format++;
            
            //gets the width in format string if there
            while (*format >= '0' && *format <= '9') {
                width = width*10 + (int) (*format - '0');
                format++;
            }
            
            //sets base if format string is for a num
            if (*format == 'd') {
                base = 10;
            } else if (*format == 'x') {
                base = 16;
            } else if (*format == 'b') {
                base = 2;
            }
            
            //adds to s according to the format string
            if (*format == 'c') {
                if (count < (n-1)){
                    *s = (char) va_arg(args, int);
                    s++;
                }
                count++;
                format++;
                
            } else if (*format == 's') {
                char *str = va_arg(args, char*);
                while (*str != '\0') {
                    if (count < (n-1)) {
                        *s = *str;
                        s++;
                    }
                    str++;
                    count++;
                }
                format++;
            } else if ((*format == 'd') || (*format =='x') || *format == 'b') {
                added = signed_to_base(s, n-count, va_arg(args, int), base, width);
                count += added;
                s += added;
                format++;
                width = 0;
            } else { //if none of the cases are there then the % is treated as just a character
                format--;
                if (count < (n-2)){
                    *s++ = *format;
                    *s++ = *format;
                }
                format +=2;
                count+=2;
            }
            
        } else {
            if (count < (n-1)){
                *s++ = *format;
            }
            format++;
            count++;
        }
    }
    
    if (n > 0) {
        *s = '\0';
    }
    
    
    
    return count;
}



int snprintf(char *s, int n, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int count = vsnprintf (s, n, format, ap);
    va_end(ap);
    
  return count;
}



int printf(const char *format, ...) {
    //gpio_write(GPIO_PIN47, 0);
    va_list ap;
    char word [1000];
    char* index = &word[0];
    va_start (ap, format);
    int count = vsnprintf(index, 1000, format, ap);
    va_end(ap);
    for (int i = 0; i < count; i++){
        uart_putc(*index);
        index++;
    }
    
  return count;
}










