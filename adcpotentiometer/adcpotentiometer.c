#define F_CPU 1000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./LCD_header.h" 

uint16_t ADC_result = 0x0000; //Used to combine the ADC-low and high result

int main(void) {
    LCD_Initialize(); 
    LCD_Send_String("Utslag:", 0, 0);
    //    LCD_Send_String("World", 5, 1); 

    //Analog- to Digital converter (ADC) setup
    //ADMUX &= ~(0xF0); //Mux 0:4 = 0 uses ADC0, that is pin C0 
    ADCSRA |= 1<<ADPS2;  //Division factor 16, that is 62.5 kHz from 1 Mhz 
    ADMUX |= 1<<REFS0; //Voltage reference: AVcc with external cap. at AREF
    //ADMUX |= 1<<ADLAR; //Align output register

    ADCSRA |= 1<<ADIE; //Enable ADC interrupts 
    ADCSRA |= 1<<ADEN; //Enable the ADC
    sei();             //Enable global interrupts
    ADCSRA |= 1<<ADSC; //Start the first conversion

    while(1) { 
    } 
}

ISR(ADC_vect) {
    //ADC_result = ((0x0000 | ADCH) << 8) | ADCL;
    ADC_result = ADCH << 8 | ADCL;
    LCD_Send_String("    ", 9, 0); 
    LCD_Send_Integer(ADC_result, 9, 0); 
    _delay_ms(200);
    ADCSRA |= 1<<ADSC; //Start new conversion
}


