#define F_CPU 12000000UL  // 12 MHz
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <avr/wdt.h> 

void WDT_off(void) {
    cli();
    wdt_reset();
    /* Clear WDRF in MCUSR */
    MCUSR &= ~(1<<WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler in setting to prevent unintentional time out */
    /* Turn off WDT */
    WDTCSR = 0x00;
    sei();
}

int main() {
    WDT_off();
        DDRB |= 1; // LED on PB0
        
        while(1) {
                PORTB |= 1; // Turn LED on
                _delay_ms(500);
                PORTB &= ~1; // Turn LED off
                _delay_ms(500);
        }

        return 1;
}
