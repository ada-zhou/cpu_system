#include "shell.h"
#include "commands.h"
#include "printf.h"
#include "uart.h"
#include "keyboard.h"
#include "reboot.h"
#include "strtol.h"
#include "console.h"


int (*shell_printf)(const char * format, ...);
void (*shell_putc)(unsigned ch);

const int commandSize = 5;

static const command_t commands[] = {
    {"help", "[cmd] prints a list of commands or description of cmd", cmd_help},
    {"echo", "<...> echos the user input to the screen", cmd_echo},
    {"reboot", "reboots your pi", cmd_reboot},
    {"peek", "takes two parameters, a required address, and a second optional number, n, then prints the n bytes or single byte if n is not supplied, in hex, that are currently at the address", cmd_peek},
    {"poke", "takes two parameters, an address and a number, sets the number to be at the address", cmd_poke},
};

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int cmd_peek(int argc, const char *argv[]){
    if (argc == 2 || argc == 3){
        const char * address = argv[1];
        char *endptr;
        unsigned long place = strtoul(address, &endptr, 0);
        if (*endptr){
            return 1;
            
        } else {
            char * byte = (char*) place;
            int n = 1;
            if (argc == 3) {
                char *numptr;
                unsigned long num = strtoul(argv[2], &numptr, 0);
                if (*numptr){
                    return 1;
                }
                n = num;
            }
           
            for (int i = 0; i < n; i++){
                shell_printf("%02x ", *(byte + i));
            }
            shell_printf("\n");
        }
        
        return 0;
    }
    
    return 1;
}

int cmd_poke(int argc, const char *argv[]){
    if (argc == 3){
        char *endptr;
        char *numptr;
        
        unsigned long address = strtoul(argv[1], &endptr, 0);
        unsigned long value = strtoul(argv[2], &numptr, 0);
        if (*endptr || *numptr){
            return 1;
        }
        *((int *)address) = (int) value;
        shell_printf("\n");
        return 0;
    }
    return 1;
}

int cmd_reboot(int argc, const char *argv[]){
    if (argc != 1){
        return 1;
    }
    reboot();
    return 0;
}

int cmd_echo(int argc, const char *argv[]) {

    for (int i = 1; i < argc; ++i) {
        if (i < argc - 1) {
            shell_printf("%s ", argv[i]);
        } else {
            shell_printf("%s", argv[i]);
        }
    }
    shell_printf("\n");
    return 0;
}

int cmd_help(int argc, const char *argv[]) {
    // Part 7.
    if (argc == 1){
        for (int i = 0; i < commandSize; i++){
            shell_printf("%s - %s\n", commands[i].name, commands[i].description);
        }
        return 0;
    } else if (argc == 2){
        for (int i = 0; i < commandSize; i++){
            if(strcmp(argv[1], commands[i].name) == 0){
                shell_printf("%s\n", commands[i].description);
                return 0;
            }
        }
    }
    
    return 1;
}

int shell_init(int graphical) {
    // Ignore graphical for now, you'll use it in
    // assignment 6.
    
    if (graphical == 1){
        console_init(40,20);
        shell_printf = console_printf;
        shell_putc = console_putc;
        
    } else {
        printf_init();
        shell_printf = printf;
        shell_putc = uart_putc;
        
    }
    return 0;
    
}

int tokenize (char * line, const char ** words, int index){
    
    words[0] = line;
    int wind = 1;
    
    for (int i = 0; i < index; i++){
        if (line[i] == ' '){
            line[i] = '\0';
            words[wind] = line+i+1;
            wind++;
        }
    }
    return wind;
    
}




void shell_run() {
    char input;
    char line [5000];
    
    
    int index = 0;
    
    while (1) {
        input = keyboard_read_char();
        if (input == '\b'){
            if (index != 0) {
                index--;
                line[index] = ' ';
                shell_printf("%c%c%c", input, line[index], input);
            }
            continue;
        }
        
        shell_printf("%c", input);
        
        if (input == '\n'){
            if (index != 0) {
                const char *words [500];
                int wind = tokenize(line, words, index);
                
                line[index] = '\0';
                
                int works = 1;
                for (int i = 0; i < commandSize; i++){
                    
                    if (strcmp(words[0], commands[i].name) == 0){
                        works = commands[i].fn(wind, words);
                        index = 0;
                    }
                }
                if (works){
                    shell_printf("Sorry invalid command :(\n");
                    index = 0;
                }
            }
            
        } else {
            if (input != '\r'){
                line[index] = input;
                index++;
            }
        }
    }
}
