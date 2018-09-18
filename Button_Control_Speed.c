// When a button is pressed, change the speed of an LED blink from 10ms to 100ms.

#include <avr/io.h>						// Include neccesary libraries
#include <util/delay.h>	

int main(void)							// Main function
{
	DDRB |= 1 << PINB0;					// Data Direction Register for PortB; sets Pin0 (LED) output
	DDRB &= ~(1 << PINB1);				// Data Direction Register for PortB; sets Pin1 (button) to input
	PORTB |= 1 << PINB1;				// Sets Pin1 to high (5v) since it is reading

	while (1)
	{
		PORTB ^= 1 << PINB0;			// XOR gate to toggle between setting Pin0 to high and low
		if (bit_is_clear(PINB, 1))		// If the button is pressed (pin0 switches to 0v)
		{
			_delay_ms(100);				// Delay of 0.1 second per toggle
		}
		else 
		{
			_delay_ms(1000);			// Delay of 1 second per toggle
		}
	}
}