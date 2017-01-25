#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h> 

int main(void) {
    //Initialize Data Direction Registry 
    DDRB = DDRB | 1 << PINB0; //Pin B0 output (1)
    //=
    //0b00000001
    
    DDRB = DDRB & ~(1 << PINB1); //Pin B1 input (0)
    //0b00000001 AND NOT 0b00000010
    //=>
    //0b00000001 AND 0b11111101
    //=
    //0b00000001

    PORTB |= 1 << PINB1; //Initialize B1 as HIGH

    while (1) {
        //Change PINB0 voltage to what it is not
        //HIGH -> LOW and LOW -> HIGH
        PORTB = PORTB ^ (1 << PINB0);

        if (bit_is_clear(PINB, 1)) {
            _delay_ms(10); //IF B1 reaches ground
        } else {
            _delay_ms(100); //If B1 disconnected
        }
    }

}
