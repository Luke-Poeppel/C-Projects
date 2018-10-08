/*
 * ADC_FirstProject.cpp
 *
 * Created: 10/8/2018 4:17:42 PM
 * Author : lukepoeppel
 */ 

#include <avr/io.h>				// AVR Library
#include <stdlib.h>				// Standard Library
#include <avr/interrupt.h>			// Interrupt Library
#include "LCDControl.h"			// LCD Library

int main(void)
{
    InitializeLCD();
	
						// Note: "ADCSRA" : ADC and Status Register
	ADCSRA |= 1 << ADPS2;			// Note: ADPS: AD Prescaler Selection
	ADMUX |= 1 << ADLAR;
	ADMUX |= 1 << REFS0;
	ADCSRA |= 1 << ADIE;
	ADCSRA |= 1 << ADEN;
	
	sei();					// Turn on interrupts
	
    while (1) 
    {
    }
}

ISR(ADC_Vect)					// Interrupt Service Routine
{
	char adcResult[4];			// 4 digit number for the ADC
	itoa(ADCH, adcResult, 10);		// Converts ADC Result Value into a string to be sent to LCD
	GoToCurrLCDLocation(1, 1);		// Find the current location of the LCD
	SendString(adcResult);			// Send itoa value to LCD
	SendString(" ");			// Include space as to not have overlap
	
	ADCSRA |= 1 << ADSC;
}

