#define F_CPU 1000000UL 
#define LCD_CONTROL_DDR DDRB
#define LCD_CONTROL_PORT PORTB
#define LCD_RS PINB0  //Register Select (H: data, L: Instruction)
#define LCD_RW PINB1  //Read-write selection (H: read, L: write)
#define LCD_E PINB2   //Enable Signal for LCM
#define LCD_DATA_DDR DDRD
#define LCD_DATA_PORT PORTD
#include <avr/io.h>
#include <util/delay.h>
#include "./LCD_header.h" 

int main(void) {
    LCD_Initialize(); 

    // xxxxxHelloxxxxxx
    // xxxxxWorldxxxxxx
    LCD_Send_String("Hello", 5, 0);
    LCD_Send_String("World", 5, 1); 

    while(1) { 
    }
}

