#define F_CPU 20000000UL
#define numberOfButtons 1 
#include <avr/io.h>
#include <util/delay.h>
#include "../ButtonPress.h"

int main(void) {
    DDRB |= 1 << PINB0; //B0 output (1)
    DDRB |= 1 << PINB1; //B1 output (1)
    DDRB &= ~(1 << PINB2); //B2 input (0)
    PORTB |= 1 << PINB1; //B0 HIGH (1) 
    PORTB |= 1 << PINB2; //B2 push-pull (1) 

    while (1) { 
        if (ButtonPressed(0, PINB, 2, 100)) {
            //0: first button
            //100 confidence
            PORTB ^= 1 << PINB0;
            PORTB ^= 1 << PINB1;
        }
    }
}
