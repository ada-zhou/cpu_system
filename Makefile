NAME = main

OBJECTS = interrupt_handlers.o

# Each libpi module must be listed in either LIBPI_STUDENT_MODULES
# or LIBPI_REFERENCE_MODULES (but not both). Student modules will be built 
# from .c files in local directory, reference modules will be obtained as
# as .o files from libpi/modules. The 11 libpi modules to be listed are:
# gpio.o timer.o printf.o backtrace.o malloc.o keyboard.o shell.o fb.o gl.o console.o circular.o
#gprof.o
LIBPI_STUDENT_MODULES = keyboard.o circular.o gpio.o printf.o backtrace.o malloc.o fb.o gl.o console.o timer.o shell.o
LIBPI_REFERENCE_MODULES = 

CS107E=../cs107e.github.io

CFLAGS  = -I$(CS107E)/libpi/include -g -Wall -Werror -Wpointer-arith 
CFLAGS += -Og -std=c99 -ffreestanding 
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name
# Note: link is now against local libmypi, not reference libpi
LDFLAGS = -nostdlib -T memmap -L.
LDLIBS = -lmypi -lgcc

REFERENCE_WITH_PATH = $(patsubst %.o,$(CS107E)/libpi/modules/%.o,$(LIBPI_REFERENCE_MODULES))
LIBPI_SOURCES = abort.c armtimer.c font.c gpioevent.c gpioextra.c interrupts.c mailbox.c ps2.c reboot.c strtol.c uart.c
LIBPI_ASM_SOURCES = register.s vectors.s
LIBPI_ALL = $(LIBPI_STUDENT_MODULES) $(REFERENCE_WITH_PATH) $(LIBPI_SOURCES:.c=.o) $(LIBPI_ASM_SOURCES:.s=.o)

all : $(NAME).bin 

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(OBJECTS) start.o cstart.o libmypi.a
	arm-none-eabi-gcc $(LDFLAGS) $(filter %.o,$^) $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

info:
	@echo "libmypi using these student modules:\033[1m $(LIBPI_STUDENT_MODULES)\033[0m"
	@echo "libmypi using these reference modules:\033[1m $(LIBPI_REFERENCE_MODULES)\033[0m"

libmypi.a: $(LIBPI_ALL) Makefile
	@make info
	@rm -f $@
	@$(CS107E)/libpi/scripts/check-conflicts $(filter %.o,$^)
	arm-none-eabi-ar cDr $@ $(filter %.o,$^)

%.list: %.o
	arm-none-eabi-objdump -d $< > $@

install: $(NAME).bin
	rpi-install.py -p $<

test: test.bin
	rpi-install.py -s $<

clean:
	rm -f *.o *.bin *.elf *.list *~ libmypi.a

.PHONY: all clean install test info

.PRECIOUS: %.o %.elf %.a

