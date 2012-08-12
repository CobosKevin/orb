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