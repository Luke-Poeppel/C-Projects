/*
 * Sept23LCD.cpp
 *
 * Created: 9/23/2018 7:10:32 AM
 * Author : lukepoeppel
 */ 

#include <avr/io.h>
#include <util/delay.h>

void CheckIfBusy(void);
void BlinkLight(void);
void SendCommand(unsigned char command);
void SendCharacter(unsigned char character);
void SendString(char *StringOfCharacters);

int main(void)
{	
	DDRD |= 1 << PIND2 | 1 << PIND5 | 1 << PIND7;	// Turn on all functions in the LCD control center (PORTD)
	_delay_ms(15);
	
	SendCommand(0x01);				// Clears the screen
	_delay_ms(2);
	SendCommand(0x38);				// Sets LCD to 8-bit, 2 lines
	_delay_ms(50);
	SendCommand(0b00001100);			// Turns on the characters (not cursor)
	_delay_ms(50);
	
	SendString("Luke W. Poeppel");			// Sends a string to the LCD using the functions below
	
    while (1)						// Run continuously
    {
    }
}

void CheckIfBusy()
// Set PORTB to input
// Turn Register Select Off
// Turn ReadWrite on.
// 
{
	DDRB = 0x00;					// Set PortB to input (data send)
	PORTD &= ~(1 << PIND2);				// Turn off Register Select (RS)
	PORTD |= 1 << PIND7;				// Turn on Read/Write (RW)
	
	while (PORTB >= 0x80)
	{
		BlinkLight();				// "Blink" from sending 5v-0v on Enable
	}
	DDRB = 0xFF;					// Set the Data Direction Registry for PortB to output
}

void BlinkLight()
// Turn on enable (PIND5) so LCD can concentrate
// Turn off enable
{
	PORTD |= 1 << PIND5;							// Turn on Enable
	asm volatile ("nop");
	asm volatile ("nop");
	PORTD &= ~(1 << PIND5);							// Turn off enable
}

void SendCommand(unsigned char command)
{
	CheckIfBusy();
	PORTB = command;								// Set PortB to command
	PORTD &= ~(1 << PIND7 | 1 << PIND2);			// Turn off Register Select (RS) and Read/Write (RW)
	BlinkLight();
	DDRB = 0;										// Set the Data Direction Register for PortB to 0. 
}

void SendCharacter(unsigned char character)
{
	CheckIfBusy();
	PORTB = character;								// Set PortB to command
	PORTD &= ~(1 << PIND7);							// Turn off Register Select
	PORTD |= 1 << PIND2;							// Turn on Read/Write (RW)
	BlinkLight();
	DDRB = 0;										// Set the Data Direction Register for PortB to 0.
}

void SendString(char *StringOfCharacters)
{
	while (*StringOfCharacters > 0)
	{
		SendCharacter(*StringOfCharacters++);		// Sends multiple characters
	}
}
