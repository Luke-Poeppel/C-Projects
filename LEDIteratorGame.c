//The Button Iterator Game Firmware

#include <avr/io.h>                                                             //Include the AVR i/o library.

                                                                                //Defining functions:
void ProcessPressedButton(int ButtonPressed);                                   //Defines a function that processes a pressed button.
void ProcessReleasedButton(int ButtonReleased);                                 //Defines a function that processes a released button.

int Pressed_Confidence_Level[2];                                                //Initializes the confidence variable for pressed button with array denoting the size (# buttons). 
int Released_Confidence_Level[2];                                               //Initializes the confidence variable for released button with array denoting the size (# buttons).
int Pressed[2];                                                                 //Initializes the pressed variable, signifying whether or not a button is pressed (# buttons).
int LEDNumber[2];                                                               //Initializes the LEDNumber variable, signifying the current LED num (# LED's).

int main(void)
{
    //Initialization Area.                                                      //DDR's and Ports
    DDRB = 0b01111111;                                                          //Data Direction Register for PortB, setting pin7 to input and pins 0-6 to output.
    DDRD = 0b01111111;                                                          //Data Direction Register for PortD, setting pin7 to input and pins 0-6 to output.
    PORTB = 0b10000000;                                                         //Sets pin7 (button) to high and the rest of the pins to low. (START WITH NO LED'S LIT.)
    PORTD = 0b10000000;                                                         //Sets pin7 (button) to high and the rest of the pins to low.

    while (1)
    {
        if (bit_is_clear(PINB, 7))
        {
            ProcessPressedButton(0);                                            //Encapsulation (see below)
        }
        else
        {
            ProcessReleasedButton(0);                                           //Encapsulation (see below)
        }
        if (bit_is_clear(PIND, 7))
        {
            ProcessPressedButton(1);                                            //Encapsulation (see below)
        }
        else
        {
            ProcessReleasedButton(1);                                           //Encapsulation (see below)
        }
    }
}

void ProcessPressedButton(int ButtonPressed)
{
    Pressed_Confidence_Level[ButtonPressed] ++;                                 //Increment the value of int Pressed_Confidence_Level (defined at the top).
        if (Pressed_Confidence_Level > 400)                                     //i.e. no mechanical debouncing on the switch.
        {
            if (Pressed[ButtonPressed] == 0)                                    //Button is released after having been pressed.
            {
                Pressed[ButtonPressed] = 1;

                if (ButtonPressed == 0) PORTB |= 1 << LEDNumber[ButtonPressed]; //If the button pressed is on portB, send a 1 (5v) to the first LED.
                if (ButtonPressed == 1) PORTD |= 1 << LEDNumber[ButtonPressed]; //If the button pressed is on portD, send a 1 (5v) to the first LED.

                LEDNumber[ButtonPressed] ++;                                    //Increment the LEDnumber upwards.

                if (LEDNumber[ButtonPressed] > 6)                               //If one port reaches pin7 faster:
                {
                    for(int i; i = 0; i < 10; i++)                              //Create a for loop for variable i, less than value of 10.
                    {
                        if (ButtonPressed == 0) PORTB = 0b10000000;             //If the button pressed is on portB, set all LED's on portB to low.
                        if (ButtonPressed == 1) PORTD = 0b10000000;             //If the button pressed is on portD, set all LED's on portB to low.
                        _delay_ms(10);                                          //Delay of 10 milliseconds.
                        if (ButtonPressed == 0) PORTB = 0b11111111;             //If the button pressed is on portB, set all LED's on portB to high.
                        if (ButtonPressed == 1) PORTD = 0b11111111;             //If the button pressed is on portD, set all LED's on portB to high.
                        _delay_ms(10);
                    }
                    LEDNumber[0] = 0;                                           //Reset the LED number on portB to 0 after the game is finished.
                    LEDNumber[1] = 0;                                           //Reset the LED number on portD to 0 after the game is finished.
                    PORTB = 0b10000000;                                         //Set the button back to high (5v) and the LED's to low on portB.
                    PORTD = 0b10000000;                                         //Set the button back to high (5v) and the LED's to low on portD.
                }
            }
        Pressed_Confidence_Level = 0;                                           //Reset the confidence level.
        }
    }
}

void ProcessReleasedButton(int ButtonReleased)
{
    Released_Confidence_Level[ButtonReleased] ++;                               //Increment the value of int Released_Confidence_Level (defined at the top).
    if (Released_Confidence_Level[ButtonReleased] > 400)                        //i.e. no mechanical debouncing on the switch.
    {    
        Pressed[ButtonReleased] = 0;                                            //That button is a 0.
        Released_Confidence_Level[ButtonReleased] = 0;                          //Reset the confidence level to 0. (Not sure why this isn't below the next bracket...)
    }
}