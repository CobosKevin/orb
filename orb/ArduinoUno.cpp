#include <Arduino.h>
#include "ArduinoUno.h"

void ArduinoUno_SetPinToOutputMode(int pin)
{
	pinMode(pin, OUTPUT);
}

void ArduinoUno_AnalogWrite(int pin, int level)
{
	analogWrite(pin, level);
}

void ArduinoUno_Delay(long ms)
{
	delay(ms);
}