#ifndef ARDUINOUNO_H
#define ARDUINOUNO_H

/*
	Thin wrapper to native Arduino functions so that they can be easily mocked
*/

void ArduinoUno_SetPinToOutputMode(int pin);
void ArduinoUno_AnalogWrite(int pin, int level);
void ArduinoUno_Delay(long ms);

#endif