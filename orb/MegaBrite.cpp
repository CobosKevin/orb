#include "MegaBrite.h"

#include <Arduino.h>

#include "ArduinoUno.h"

#define LED_FADE_STEP_SLOW  1
#define LED_FADE_STEP_FAST  16

#define FADE_UP         1
#define FADE_DOWN       -1

#define CLOCK_PIN  13U
#define ENABLE_PIN 10U
#define LATCH_PIN   9U
#define DATA_PIN   11U

MegaBrite::MegaBrite()
{
	ArduinoUno_SetPinToOutputMode(CLOCK_PIN);
	ArduinoUno_SetPinToOutputMode(ENABLE_PIN);
	ArduinoUno_SetPinToOutputMode(LATCH_PIN);
	ArduinoUno_SetPinToOutputMode(DATA_PIN);

	SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
	ArduinoUno_DigitalWrite(LATCH_PIN, LOW);
	ArduinoUno_DigitalWrite(ENABLE_PIN, LOW);

	for (int i =0; i < 3; i++)
	{
		pulse[i] = false;
		toggle[i] = false;
		step[i] = LED_FADE_STEP_SLOW;
		value[i] = 0;
		direction[i] = FADE_UP;
	}	

	setControlRegister();
	setPWMRegister();
}


void MegaBrite::pulseOn(unsigned short led, unsigned short speed)
{
	if (speed == SLOW)
	{
		step[led] = LED_FADE_STEP_SLOW;
	}
	else
	{
	    step[led] = LED_FADE_STEP_FAST;		
	}
	toggleOff(led);
	value[led] = 0;
	direction[led] = FADE_UP;
	pulse[led] = true;
}

void MegaBrite::pulseOff(unsigned short led)
{
	pulse[led] = false;
	value[led] = 0;
}

void MegaBrite::toggleOn(unsigned short led)
{
	pulseOff(led);
	value[led] = 0;
	toggle[led] = true;
}

void MegaBrite::toggleOff(unsigned short led)
{
	toggle[led] = false;
	value[led] = 0;
}

void MegaBrite::on(unsigned short led)
{
	pulseOff(led);
	toggleOff(led);
	value[led] = 1023;
	setPWMRegister();
}

void MegaBrite::off(unsigned short led)
{
	pulseOff(led);
	toggleOff(led);
	value[led] = 0;
	setPWMRegister();
}

void MegaBrite::animate()
{
	bool toggled = false;
    static long lastToggled = 0;

	for (int i =0; i < 3; i++)
	{
	    if (pulse[i])
	    {
	        value[i] += (direction[i] * step[i]);	        
	        if (value[i] >= 1024)
	        {
	        	value[i] = 1023;
	            direction[i] = FADE_DOWN;
	        }
	        if (value[i] <= 0)
	        {
	        	value[i] = 0;
	            direction[i] = FADE_UP;
	        }
	    }		
	}

	if (millis() - lastToggled > 1000)
	{
		for (int i =0; i < 3; i++)
		{
		    if (toggle[i])
		    {
		        if (value[i] == 0 && !toggled)
		        {
		        	value[i] = 1023;
		        	toggled = true;
	                lastToggled = millis();
		        }
		        else
		        {
		        	value[i] = 0;
		        }
		    }		
		}		
	}

	setPWMRegister();
}

void MegaBrite::setPWMRegister()
{
	static int previousRed = -1;
	static int previousGreen = -1;
	static int previousBlue = -1;

	if (previousRed != value[RED] || previousGreen != value[GREEN] || previousBlue != value[BLUE])
	{
		// Write to the PWM registers
		sendPacket(B00, value[RED], value[GREEN], value[BLUE]);
		delayMicroseconds(15);
	    ArduinoUno_DigitalWrite(LATCH_PIN, HIGH); // LATCH_PIN data into registers
	    delayMicroseconds(15);
	    ArduinoUno_DigitalWrite(LATCH_PIN, LOW);

	    previousRed = value[RED];
	    previousGreen = value[GREEN];
	    previousBlue = value[BLUE];
	}
}

/*
	Send command to set control register.
	http://docs.macetech.com/doku.php/megabrite
*/
void MegaBrite::setControlRegister()
{
	sendPacket(B01, 127, 100, 100);
	delayMicroseconds(15);
    ArduinoUno_DigitalWrite(LATCH_PIN, HIGH); // LATCH_PIN data into registers
    delayMicroseconds(15);
    ArduinoUno_DigitalWrite(LATCH_PIN, LOW);    
}

void MegaBrite::sendPacket(int mode, int red, int green, int blue) 
{ 
    SPDR = mode << 6 | blue >> 4;
    while(!(SPSR & (1<<SPIF)));
    SPDR = blue << 4 | red >> 6;
    while(!(SPSR & (1<<SPIF)));
    SPDR = red << 2 | green >> 8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = green;
    while(!(SPSR & (1<<SPIF)));
}
