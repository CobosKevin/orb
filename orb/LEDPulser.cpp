#include "LEDPulser.h"

#include "ArduinoUno.h"

#define LED_FADE_STEP_SLOW 	1
#define LED_FADE_STEP_FAST 	15

#define FADE_UP 		1
#define FADE_DOWN 		-1

LEDPulser::LEDPulser(int ledPin)
{	
	ArduinoUno_SetPinToOutputMode(ledPin);
	level = 0;
	direction = FADE_UP;
	pin = ledPin;	
	step = LED_FADE_STEP_SLOW;
	fPulse = false;
}

void LEDPulser::pulse()
{
	if (fPulse) {
	    level += (direction * step);
	    ArduinoUno_AnalogWrite(pin, level);
	    if (level == 255) {
	    	direction = FADE_DOWN;
	    }
	    if (level == 0) {
	    	direction = FADE_UP;
	    }
	 }	
}

void LEDPulser::enable()
{
	fPulse = true;
}

void LEDPulser::disable()
{
	fPulse = false;
}

void LEDPulser::slow()
{
	step = LED_FADE_STEP_SLOW;
}

void LEDPulser::fast()
{
	step = LED_FADE_STEP_FAST;
}

void LEDPulser::on()
{
	disable();
    ArduinoUno_AnalogWrite(pin, 255);
}

void LEDPulser::off()
{
	disable();
    ArduinoUno_AnalogWrite(pin, 0);
}
