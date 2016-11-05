#define F_CPU 8000000UL
#include <avr/io.h>

int main(void) {
    DDRD = 0b00111111; //Output D0:D6
    PORTD = 0b00000000; //D0:D6 LOW
    TCCR1B |= 1<<CS12; //set 256 prescaler clkdiv
    int LEDNumber = 0; //LED to shine
    while(1) {
        if (TCNT1 > 5208/12) { //8 Mhz / 256 / 6
            TCNT1 = 0; // Reset counter from 5208 to 0
            PORTD = 1 << LEDNumber; //Turn on only LEDnumber on D
            LEDNumber++; //Move to next LED
            if (LEDNumber > 6) { //If past last led (5)
                LEDNumber = 0; //Start at first LED (0)
                PORTD = 1 << LEDNumber;
                LEDNumber++;
            }
        }
    }
}
