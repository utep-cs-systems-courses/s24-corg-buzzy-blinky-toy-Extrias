

COMMON_OBJECTS = main.o buzzer.o
CPU	       = msp430g2553
CFLAGS	       = -mmcu=${CPU} -I../h
LDFLAGS	       = -L/opt/ti/msp430_gcc/include


#switch the compiler for internal make rules
CC 		= msp430-elf-gcc
AS		= msp430-elf-as


all: led.elf


#additional rules for files
led.elf: ${COMMON_OBJECTS} led.h
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a



load: led.elf
	msp430loader.sh led.elf


clean:
	rm -f *.o *.elf


main.o: led.h buzzer.h

buzzer.o: buzzer.c buzzer.h
