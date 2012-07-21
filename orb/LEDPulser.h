#ifndef LEDPULSER_H
#define LEDPULSER_H

class LEDPulser
{
  public:
    LEDPulser() {};
    LEDPulser(int ledPin);

    void pulse();
    void enable();
    void disable();
    void slow();
    void fast();
    void on();
    void off();
    
  private:
    int pin;
    int direction;
    int level;
    int step;
    bool fPulse;
};

#endif
