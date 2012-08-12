#ifndef ARDUINOUNO_H
#define ARDUINOUNO_H

/*
	Thin wrapper to native Arduino functions so that they can be easily mocked
*/

#ifndef Arduino_h
typedef unsigned char uint8_t;	
typedef uint8_t byte;
#endif

void ArduinoUno_SetPinToOutputMode(unsigned short pin);
void ArduinoUno_AnalogWrite(unsigned short pin, unsigned short level);
void ArduinoUno_DigitalWrite(unsigned short pin, unsigned short level);
void ArduinoUno_Delay(long ms);

#endif