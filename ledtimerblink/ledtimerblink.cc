#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
  
int main(void) {
    DDRD |= (1 << 0);
  
    // initialize timer
    // set up timer with prescaler = 64 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10); 
    // initialize counter
    TCNT1 = 0; 
    // initialize compare value 
    OCR1A = 8000000 / 64 / 8; //1 second 
    // enable compare interrupt
    TIMSK1 |= (1 << OCIE1A); 
    // enable global interrupts
    sei();
  
    // loop forever
    while(1) {
    }
}

ISR (TIMER1_COMPA_vect) { 
    PORTD ^= (1 << 0); //Toggle LED on and off
}
