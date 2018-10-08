/*
 * ADC_FirstProject.cpp
 *
 * Created: 10/8/2018 4:17:42 PM
 * Author : lukepoeppel
 */ 

#include <avr/io.h>				// AVR Library
#include <stdlib.h>				// Standard Library
#include <avr/interrupt.h>			// Interrupt Library
#include "LCDControl.h"				// LCD Library

int main(void)
{
    InitializeLCD();
	
	// Notes: "ADCSRA" : ADC and Status Register, "ADPS" : AD Prescaler Selection, "ADMUX" : AD Multiplexer Selection,
	// (cont.) "REFSx" : Voltage Reference for the ADC, "ADLAR" : Presentation of ADC Result, "ADIE" : Enable the Interrupt, 
	// (cont.) "ADEN" : Enable the ADC.
	
	ADCSRA |= 1 << ADPS2;			// Enable the Prescalar Selection
	ADMUX |= 1 << ADLAR;			// Enable the presentation of the ADC on the Multiplexer
	ADMUX |= 1 << REFS0;			// Enable the voltage selection of the ADC on the Multiplexer
	ADCSRA |= 1 << ADIE;			// Enable the interrupt on the ADC
	ADCSRA |= 1 << ADEN;			// Enable the ADC!
	
	sei();					// Enable interrupts
	
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
	
	ADCSRA |= 1 << ADSC;			// Enable the "ADC Start Conversion" Pin
}
