#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= 1 << PINB0; //B0 output (1)
    DDRB |= 1 << PINB1; //B1 output (1)
    DDRB &= ~(1 << PINB2); //B2 input (0)
    PORTB |= 1 << PINB2; //B2 push-pull (1)
    PORTB |= 1 << PINB0;

    int pressed = 0;
    int pressed_confidence = 0;
    int released_confidence = 0;

    while (1) { 
        if (bit_is_clear(PINB, 2)) {
            pressed_confidence++;
            released_confidence = 0;
            if (pressed_confidence > 500) { 
                if (!pressed) {
                    PORTB ^= 1 << PINB0; //Toggle LED in pin 0
                    PORTB ^= 1 << PINB1; //Toggle LED on pin 1
                    pressed = 1;
                }
                pressed_confidence = 0;
            }

        }
        else {
            pressed_confidence = 0;
            released_confidence++;
            if (released_confidence > 500) {
                pressed = 0; 
                released_confidence = 0;
            }

        }
    }
}
