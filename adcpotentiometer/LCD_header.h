#ifndef LCD_header
#define LCD_header

///DEVICE SPECIFIC SETUP///
#define LCD_CONTROL_DDR DDRB
#define LCD_CONTROL_PORT PORTB
#define LCD_RS PINB0  //Register Select (H: data, L: Instruction)
#define LCD_RW PINB1  //Read-write selection (H: read, L: write)
#define LCD_E PINB2   //Enable Signal for LCM
#define LCD_DATA_DDR DDRD
#define LCD_DATA_PORT PORTD 

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h> 

void LCD_Wait_Until_Ready();
void LCD_Flash_Enable_Signal();
void LCD_Send_Command(char command);
void LCD_Send_Character(char character);
void LCD_Send_String(char *string, unsigned char col, unsigned char row);
void LCD_Move_Cursor(unsigned char col, unsigned char row);

void LCD_Initialize() {
    LCD_CONTROL_DDR |= 1<<LCD_RS | 1<<LCD_RW | 1<<LCD_E; 
    _delay_ms(20);
    LCD_Send_Command(0x38); // 8 bit mode 
    LCD_Send_Command(0x0E); // Clear screen 
    LCD_Send_Command(0x01); // Display on cursor on 
    _delay_ms(2);
}

/**Check if busy
 * Set the microcontroller port data direction to input.
 * Set the pin connected to the LCD RW to ON (read mode)
 * Set the pin connected to the LCD RS to OFF (cmd mode)
 * Turn the enable on, then off after a tiny delay.
 * Read the LCD_DATA_PORT pin 7 to determine if it is a 1 or a 0.
 * If it is 1 it is busy, 0 if ready
 */ 
void LCD_Wait_Until_Ready() {
    LCD_DATA_DDR = 0; //Whole port as input (Read)
    LCD_CONTROL_PORT |= 1<<LCD_RW; //LCD Read mode
    LCD_CONTROL_PORT &= ~(1<<LCD_RS); //LCD Instruction register
    while (LCD_CONTROL_PORT >= 0x80) { //True until pin7 is 1
        LCD_Flash_Enable_Signal();
    }
    LCD_DATA_DDR = 0xFF; //Whole port as output 
}

/**Flash enable signal for LCM*/ 
void LCD_Flash_Enable_Signal() {
    LCD_CONTROL_PORT |= 1<<LCD_E;
    asm volatile ("nop"); //Assembly command wait
    asm volatile ("nop"); 
    LCD_CONTROL_PORT &= ~(1<<LCD_E); 
}

/*Send command/character
 * Make sure LCD is ready
 * Instruction register for command, data register for character
 * Read/Write set to low for write mode
 * Data port as output equal to command or character
 * Flash enable port
 */ 
void LCD_Send_Command(char command) {
    LCD_Wait_Until_Ready();
    LCD_DATA_PORT = command;
    LCD_CONTROL_PORT &= ~(1<<LCD_RS); //LCD Instruction register
    LCD_CONTROL_PORT &= ~(1<<LCD_RW); //LCD Write mode
    LCD_Flash_Enable_Signal();
    LCD_DATA_PORT = 0x00; 
}

void LCD_Send_Character(char character) {
    LCD_Wait_Until_Ready();
    LCD_DATA_PORT = character;
    LCD_CONTROL_PORT |= 1<<LCD_RS; //LCD Data register
    LCD_CONTROL_PORT &= ~(1<<LCD_RW); //LCD Write mode
    LCD_Flash_Enable_Signal();
    LCD_DATA_PORT = 0x00; 
}

void LCD_Send_String(char *string, unsigned char col, unsigned char row) {
    LCD_Move_Cursor(col,row);
    while (*string != 0x00) {
        LCD_Send_Character(*string++);
    }
}

void LCD_Move_Cursor(unsigned char col, unsigned char row) {
    LCD_Send_Command(0x80 + col + row*64) ;
}

void LCD_Send_Integer(int integer, unsigned char col, unsigned char row) {
    //Maximum 5 digits with prepended sign
    //ex -10000
    char integer_string[7] = {0, 0, 0, 0, 0, 0, 0}; 
    itoa(integer, integer_string, 10);
    LCD_Send_String(integer_string, col, row); 
}

void LCD_Send_Decimal(double decimal, unsigned char col, unsigned char row) {
    //Maximum 5 digits with prepended sign and decimal mark
    //ex -132.55
    char decimal_string[8] = {0, 0, 0, 0, 0, 0, 0}; 
    dtostrf(decimal, 7, 3, decimal_string);
    LCD_Send_String(decimal_string, col, row); 
}


#endif
