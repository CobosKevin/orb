#ifndef LEDPULSER_H
#define LEDPULSER_H

#include "ArduinoUno.h"

class LEDPulser
{
public:
    LEDPulser() {};
    LEDPulser(byte ledPin);

    void pulse();
    void enable();
    void disable();
    void slow();
    void fast();
    void on();
    void off();

private:
    byte pin;
    byte direction;
    unsigned short level;
    byte step;
    bool fPulse;
};

#endif
