#include <Arduino.h>
#include "ArduinoUno.h"

void ArduinoUno_SetPinToOutputMode(unsigned short pin)
{
    pinMode(pin, OUTPUT);
}

void ArduinoUno_AnalogWrite(unsigned short pin, unsigned short level)
{
    analogWrite(pin, level);
}

void ArduinoUno_DigitalWrite(unsigned short pin, unsigned short level)
{
    digitalWrite(pin, level);
}

void ArduinoUno_Delay(long ms)
{
    delay(ms);
}

void ArduinoUno_DelayMicroseconds(long us)
{
	delayMicroseconds(us);
}

long ArduinoUno_Millis()
{
	return millis();
}

void ArduinoUno_SetSPIControlRegister(byte value)
{
	SPCR = value;
}

void ArduinoUno_SetSPIDataRegister(byte value)
{
	SPDR = value;
}

byte ArduinoUno_GetSPIStatusRegister()
{
	return SPSR;
}
